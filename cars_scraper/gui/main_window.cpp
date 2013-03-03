#include "stdafx.h"
#include "main_window.h"
#include "main_window_moc.h"
#include "impl/offer_table_model.h"

namespace gui
{
   main_window_t::main_window_t ()
      : scraper_ ( this )
      , offers_founded_ ( 0u )
      , offers_processed_ ( 0u )
   {
      ui_.setupUi(this);

      QSortFilterProxyModel * proxy = new QSortFilterProxyModel(ui_.table);
      proxy->setSourceModel(new impl::offers_table_model_t(proxy));

      ui_.table->setModel(proxy);

      QObject::connect(&scraper_, SIGNAL(started()),
                       this, SLOT(on_started()));
      QObject::connect(&scraper_, SIGNAL(finished(bool)),
                       this, SLOT(on_finished(bool)));

      QObject::connect(&scraper_, SIGNAL(offer_processed(scraper::offer_t const&)),
                       this, SLOT(on_offer_processed(scraper::offer_t const&)));
      QObject::connect(&scraper_, SIGNAL(offers_founded(unsigned)),
                       this, SLOT(on_offers_founded(unsigned)));
   }

   // gui slots
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::on_searching_start ()
   {
      scraper_.start();
   }

   void main_window_t::on_searching_stop ()
   {
      scraper_.stop();
   }

   void main_window_t::on_clear ()
   {
      model()->rem_offers();
      update();
   }

   void main_window_t::on_show_offer ( QModelIndex const& index )
   {
      scraper::offer_t const& offer =
         model()->get_offer(proxy()->mapToSource(index).row());

      QWebView * view = new QWebView();
      view->setWindowTitle(utils::to_qt(offer.model));

      view->setWindowModality(Qt::ApplicationModal);
      view->setAttribute(Qt::WA_DeleteOnClose, true);

      view->load(offer.url);
      view->show();
   }

   // scraper slots
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::on_started ()
   {
      ui_.searching_start->setEnabled(false);
      ui_.searching_stop ->setEnabled(true);

      offers_founded_   = 0u;
      offers_processed_ = 0u;

      update();
   }

   void main_window_t::on_finished ( bool canceled )
   {
      ui_.searching_start->setEnabled(true);
      ui_.searching_stop ->setEnabled(false);

      offers_founded_   = 0u;
      offers_processed_ = 0u;

      update();
   }

   void main_window_t::on_offer_processed ( scraper::offer_t const& offer )
   {
      ui_.table->setSortingEnabled(false);
      model()->add_offer(offer);
      ui_.table->setSortingEnabled(true);

      ++offers_processed_;

      update();
   }

   void main_window_t::on_offers_founded ( unsigned count )
   {
      offers_founded_ = count;
      update();
   }

   // impl
   //////////////////////////////////////////////////////////////////////////
   QAbstractProxyModel * main_window_t::proxy () const
   {
      return dynamic_cast<QAbstractProxyModel *>(ui_.table->model());
   }

   impl::offers_table_model_t * main_window_t::model () const
   {
      return dynamic_cast<impl::offers_table_model_t *>(proxy()->sourceModel());
   }

   void main_window_t::update ()
   {
      if (offers_founded_ != 0)
      {
         int const progress =
            static_cast<int>(100. * offers_processed_ / offers_founded_);
         ui_.progress->setValue(progress);

         static wformat fmt (L"ќбъ€влений обработано: %d/%d");
         ui_.status->showMessage(utils::to_qt(str(fmt % offers_processed_ % offers_founded_)));
      }
      else
      {
         ui_.progress->setValue(100);

         static wformat fmt (L"ќбъ€влений найдено: %d");
         ui_.status->showMessage(utils::to_qt(str(fmt % model()->offers_count())));
      }

      ui_.clear->setEnabled(model()->offers_count() > 0u);
   }
}