#include "stdafx.h"
#include "main_window.h"
#include "main_window_moc.h"

namespace gui
{
   namespace
   {
      enum column_t
      {
         column_model = 0,
         column_price,
         column_year,
         column_run
      };

      template<class T>
         QTableWidgetItem * create_item ( T const& val, int aligment )
      {
         QTableWidgetItem * item = new QTableWidgetItem();
         item->setData(Qt::DisplayRole, utils::to_variant(val));
         item->setTextAlignment(aligment);
         return item;
      }

      void add_offer_to_table ( QTableWidget * table, wstring const& offer_id,
         scraper::offer_t const& offer )
      {
         table->setSortingEnabled(false);

         int const row = table->rowCount();
         table->insertRow(row);

         QTableWidgetItem * item =
            create_item(offer.model, Qt::AlignLeft | Qt::AlignVCenter);
         item->setData(Qt::UserRole, utils::to_variant(offer_id));
         table->setItem(row, column_model, item);

         if (offer.year)
            table->setItem(row, column_year,
               create_item(*offer.year,  Qt::AlignHCenter | Qt::AlignVCenter));
         if (offer.price)
            table->setItem(row, column_price,
               create_item(*offer.price, Qt::AlignRight | Qt::AlignVCenter));
         if (offer.run)
            table->setItem(row, column_run,
               create_item(*offer.run,   Qt::AlignRight | Qt::AlignVCenter));

         table->setSortingEnabled(true);
      }
   }

   main_window_t::main_window_t ()
      : scraper_ ( this )
      , offers_founded_ ( 0u )
      , offers_processed_ ( 0u )
   {
      QObject::connect(&scraper_, SIGNAL(started()),
                       this, SLOT(on_started()));
      QObject::connect(&scraper_, SIGNAL(finished(bool)),
                       this, SLOT(on_finished(bool)));

      QObject::connect(&scraper_, SIGNAL(offer_processed(scraper::offer_t const&)),
                       this, SLOT(on_offer_processed(scraper::offer_t const&)));
      QObject::connect(&scraper_, SIGNAL(offers_founded(unsigned)),
                       this, SLOT(on_offers_founded(unsigned)));
      ui_.setupUi(this);
   }

   // gui slots
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::on_find_start ()
   {
      scraper_.start();
   }

   void main_window_t::on_find_stop ()
   {
      scraper_.stop();
   }

   void main_window_t::on_clear ()
   {
      offers_.clear();

      ui_.table->setRowCount(0);
      ui_.clear->setEnabled(false);

      ui_.status->showMessage(utils::to_qt(L""));
   }

   void main_window_t::on_show_offer ( int row, int column )
   {
      wstring const offer_id =
         utils::from_variant<wstring>(ui_.table->item(row, 0)->data(Qt::UserRole));
      offers_t::const_iterator const i =
         offers_.find(offer_id);

      if (i != offers_.end())
      {
         scraper::offer_t const& offer = i->second;

         QWebView * view = new QWebView();
         view->setWindowTitle(utils::to_qt(offer.model));

         view->setWindowModality(Qt::ApplicationModal);
         view->setAttribute(Qt::WA_DeleteOnClose, true);

         view->load(offer.url);
         view->show();
      }
      else
         assert(false);
   }

   // scraper slots
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::on_started ()
   {
      ui_.find_start->setEnabled(false);
      ui_.find_stop ->setEnabled(true);

      offers_founded_   = 0u;
      offers_processed_ = 0u;
   }

   void main_window_t::on_finished ( bool canceled )
   {
      ui_.find_start->setEnabled(true);
      ui_.find_stop ->setEnabled(false);

      ui_.progress->setValue(100);

      static wformat fmt (L"ќбъ€влений найдено: %d");
      ui_.status->showMessage(utils::to_qt(str(fmt % offers_.size())));
   }

   void main_window_t::on_offer_processed ( scraper::offer_t const& offer )
   {
      optional<wstring> const offer_id =
         scraper::get_offer_id(offer.url);
      assert(offer_id);

      if (offer_id && offers_.find(*offer_id) == offers_.end())
      {
         offers_.insert(std::make_pair(*offer_id, offer));
         add_offer_to_table(ui_.table, *offer_id, offer);

         ui_.clear->setEnabled(true);
      }

      ++offers_processed_;
      update_progress();
   }

   void main_window_t::on_offers_founded ( unsigned count )
   {
      offers_founded_ = count;
      update_progress();
   }

   // impl
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::update_progress ()
   {
      int const progress =
         static_cast<int>(100. * offers_processed_ / offers_founded_);
      ui_.progress->setValue(progress);

      static wformat fmt (L"ќбъ€влений обработано: %d/%d");
      ui_.status->showMessage(utils::to_qt(str(fmt % offers_processed_ % offers_founded_)));
   }
}