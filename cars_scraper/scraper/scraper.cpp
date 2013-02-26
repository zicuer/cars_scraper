#include "stdafx.h"

#include "scraper.h"
#include "scraper_moc.h"

namespace scraper
{
   namespace
   {
      bool filter ( QUrl const& url )
      {
         QString const path = url.path();
         return (url.host().contains(utils::to_qt(L"auto.ru"), Qt::CaseInsensitive) &&
                 !path.endsWith(utils::to_qt(L"css"), Qt::CaseInsensitive) &&
                 !path.endsWith(utils::to_qt(L"php"), Qt::CaseInsensitive) &&
                 !path.endsWith(utils::to_qt(L"jpg"), Qt::CaseInsensitive) &&
                 !path.endsWith(utils::to_qt(L"png"), Qt::CaseInsensitive) &&
                 !path.endsWith(utils::to_qt(L"gif"), Qt::CaseInsensitive) &&
                 !path.endsWith(utils::to_qt(L"js"),  Qt::CaseInsensitive));
      }

      static QUrl const search_page_url (utils::to_qt(L"http://all.auto.ru/extsearch/"));
      static double const loading_timeout = 10.;

      static unsigned const max_timeouts_count = 3u;
      static unsigned const max_captchas_count = 3u;
   }

   using namespace impl;

   scraper_t::scraper_t ( QObject * parent )
      : QObject ( parent )
      , page_ ( new page_t(this, bind(&filter, _1)) )
   {
      QObject::connect(page_, SIGNAL(loaded(bool)),
                       this, SLOT(on_loaded(bool)));
      QObject::connect(page_, SIGNAL(timeout()),
                       this, SLOT(on_timeout()));
      QObject::connect(page_, SIGNAL(canceled()),
                       this, SLOT(stop()));
   }

   // public slots
   //////////////////////////////////////////////////////////////////////////
   void scraper_t::start ()
   {
      assert(requests_queue_.empty() &&
             !request_);

      request_ =
         request_t(search_page_url, rt_offers);

      page_->load(request_->url, none);
      page_->set_visible(true);
   }

   void scraper_t::stop ()
   {
      assert(request_);

      page_->set_visible(false);
      page_->stop();

      requests_queue_.clear();
      request_.reset();

      emit(finished(true));
   }

   // private slots
   //////////////////////////////////////////////////////////////////////////
   void scraper_t::on_loaded ( bool ok )
   {
      if (!ok)
         return;


   }

   void scraper_t::on_timeout ()
   {
      assert(request_);
      if (!page_->is_visible())
      {
         if (++request_->timeouts_count == max_timeouts_count)
            stop();
         else
         {
            stop_current_request(false);
            process_next_request();
         }
      }
   }

   // impl
   //////////////////////////////////////////////////////////////////////////
   void scraper_t::stop_current_request ( bool processed )
   {
      assert(request_);

      page_->set_visible(false);
      page_->stop();

      if (!processed)
         requests_queue_.push_front(*request_);

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

         page_->load(request_->url,
            loading_timeout);
      }
   }
}