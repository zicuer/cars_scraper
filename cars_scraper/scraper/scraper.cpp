#include "stdafx.h"

#include "scraper.h"
#include "scraper_moc.h"

namespace scraper
{
   using namespace impl;
   using namespace utils;

   namespace
   {
      bool filter ( QUrl const& url )
      {
         QString const path = url.path();
         return (url.host().contains(to_qt(L"auto.ru"), Qt::CaseInsensitive) &&
                 !path.endsWith(to_qt(L"css"), Qt::CaseInsensitive) &&
                 !path.endsWith(to_qt(L"php"), Qt::CaseInsensitive) &&
                 !path.endsWith(to_qt(L"jpg"), Qt::CaseInsensitive) &&
                 !path.endsWith(to_qt(L"png"), Qt::CaseInsensitive) &&
                 !path.endsWith(to_qt(L"gif"), Qt::CaseInsensitive) &&
                 !path.endsWith(to_qt(L"js"),  Qt::CaseInsensitive));
      }
   }

   scraper_t::scraper_t ( QObject * parent )
      : QObject ( parent )
      , page_ ( new page_t(this, bind(&filter, _1)) )
   {
      QObject::connect(page_, SIGNAL(loaded(bool)),
                       this, SLOT(on_loaded(bool)));
      QObject::connect(page_, SIGNAL(timeout()),
                       this, SLOT(on_timeout()));
      QObject::connect(page_, SIGNAL(canceled()),
                       this, SLOT(pause()));
   }

   state_t scraper_t::state () const
   {
      if (request_)
         return state_started;

      if (!requests_queue_.empty())
         return state_paused;

      return state_stopped;
   }

   // public slots
   //////////////////////////////////////////////////////////////////////////
   void scraper_t::start ()
   {
      assert(requests_queue_.empty() &&
             !request_);

      QUrl const search_page_url =
         to_url(settings_t().get_value<QString>(L"scraper/search_page"));
      requests_queue_.
         push_front(request_t(search_page_url, rt_search));
      process_next_request();

      emit(started());
   }

   void scraper_t::stop ()
   {
      assert(request_);

      stop_current_request(false);
      requests_queue_.clear();

      emit(finished(true));
   }

   void scraper_t::pause ()
   {
      assert(request_);

      stop_current_request(false);
      emit(paused(true));
   }

   void scraper_t::resume ()
   {
      assert(!requests_queue_.empty());

      process_next_request();
      emit(resumed());
   }

   // private slots
   //////////////////////////////////////////////////////////////////////////
   void scraper_t::on_loaded ( bool ok )
   {
      if (!ok)
         return;

      assert(request_);
      if (!check_captcha())
      {
         bool processed = false;
         switch (request_->type)
         {
         case rt_search:
         case rt_offers: { processed = process_offers_request(); } break;
         case rt_offer:  { processed = process_offer_request();  } break;;
         case rt_phone_numbers:
                         { processed = process_phones_request(); } break;;
         }

         if (processed)
         {
            stop_current_request(processed);
            process_next_request();
         }
      }
   }

   void scraper_t::on_timeout ()
   {
      assert(request_);
      if (!page_->is_visible())
      {
         unsigned const max_timeouts_count =
            settings_t().get_value<unsigned>(L"scraper/max_timeouts_count");

         stop_current_request(false);
         if (++request_->timeouts_count >=
             max_timeouts_count)
         {
           request_->timeouts_count = 0u;
           emit(paused(false));
         }
         else
            process_next_request();
      }
   }

   // impl
   //////////////////////////////////////////////////////////////////////////
   void scraper_t::stop_current_request ( bool processed )
   {
      assert(request_);

      if (page_->is_visible())
         page_->set_visible(false);

      page_->stop();

      if (!processed)
      {
         if (request_->type == rt_phone_numbers)
         {
            offer_t const* offer
               = request_->data<offer_t>();
            assert(offer);

            request_->url = offer->url;
            request_->type = rt_offer;
         }

         requests_queue_
            .push_front(*request_);
      }

      request_.reset();
   }

   void scraper_t::process_next_request ()
   {
      assert(!request_);

      if (requests_queue_.empty())
         emit(finished(false));
      else
      {
         request_ = requests_queue_.front();
         requests_queue_.pop_front();

         if (request_->type == rt_search)
         {
            page_->load(request_->url, none);
            page_->set_visible(true);
         }
         else
         {
            double const loading_timeout =
               settings_t().get_value<double>(L"scraper/loading_timeout");
            page_->load(request_->url,
               loading_timeout);
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   bool scraper_t::check_captcha ()
   {
      assert(request_);
      if (!page_->find_any(L"#captchaInput"))
         return false;

      if (!page_->is_visible())
      {
         unsigned const max_captchas_count =
            settings_t().get_value<unsigned>(L"scraper/max_captchas_count");

         unsigned const captchas_count =
            ++request_->captchas_count;
         stop_current_request(false);

         if (captchas_count >=
               max_captchas_count)
            page_->set_visible(true);
         else
            page_->reset();

         process_next_request();
      }
      return true;
   }

   bool scraper_t::process_offers_request ()
   {
      assert(request_ && ((request_->type == rt_search) ||
                          (request_->type == rt_offers)));
      vector<QWebElement> const offers =
         page_->find_all(L"a.offer-list");

      if (offers.empty())
         return false;

      for each (QWebElement const& offer in offers)
      {
         QUrl const offer_url =
            to_url(offer.attribute(to_qt(L"href")));
         requests_queue_.push_back(request_t(offer_url, rt_offer));
      }

      if (optional<QWebElement> const next_page =
            page_->find_any(L"[title=Следующая]"))
      {
         QUrl const next_page_url =
            request_->url.resolved(to_url(next_page->attribute(to_qt(L"href"))));
         requests_queue_.push_back(request_t(next_page_url, rt_offers));
      }

      if (optional<QWebElement> const counter =
            page_->find_any(L"div.cell-2|span"))
      {
         try
         {
            unsigned const offers_count =
               lexical_cast<unsigned>(from_qt(counter->toPlainText()));
            emit(offers_founded(offers_count));
         }
         catch (bad_lexical_cast const&)
         {}
      }

      return true;
   }

   bool scraper_t::process_offer_request ()
   {
      assert(request_ && (request_->type == rt_offer));

      optional<QWebElement> const model =
         page_->find_any(L"a.auto-model_link");

      if (!model)
         return false;

      offer_t offer;
      offer.url = request_->url;
      offer.model = from_qt(model->toPlainText());

      if (optional<QWebElement> const year =
            page_->find_any(L"dd#card-year|strong"))
         offer.year = get_year(year->toPlainText());

      if (optional<QWebElement> const run =
            page_->find_any(L"dd#card-run|strong"))
         offer.run = get_run(run->toPlainText());

      if (optional<QWebElement> const price =
            page_->find_any(L"p.cost|strong"))
         offer.price = get_price(price->toPlainText());

      if (optional<QWebElement> const get_phones =
            page_->find_any(L"span#get-sale-phones"))
      {
         QUrl const phones_url =
            request_->url.resolved(to_url(get_phones->attribute(to_qt(L"rel"))));

         request_t phones_request (phones_url, rt_phone_numbers);
         phones_request.set_data<offer_t>(offer);

         requests_queue_.push_front(phones_request);
      }
      else
      {
         request_->set_data<offer_t>(offer);
         process_phones_request();
      }

      return true;
   }

   bool scraper_t::process_phones_request ()
   {
      assert(request_ && ((request_->type == rt_offer) ||
                          (request_->type == rt_phone_numbers)));

      vector<QWebElement> const phones =
         page_->find_all(L"ul.sale-phones|strong");

      if (phones.empty())
         return false;

      offer_t * offer = request_->data<offer_t>();
      assert(offer);

      for each (QWebElement const& phone in phones)
         if (optional<phone_number_t> const offer_phone =
               get_phone_number(phone.toPlainText()))
            offer->phone_numbers.push_back(*offer_phone);

      emit(offer_processed(*offer));
      return true;
   }
}