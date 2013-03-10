#include "stdafx.h"
#include "main_window.h"
#include "main_window_moc.h"
#include "impl/offers_table_model.h"

namespace gui
{
   namespace
   {
      wstring statrt_button_text ( scraper::state_t state )
      {
         switch (state)
         {
         case scraper::state_stopped:
            return L"Найти";
         case scraper::state_started:
            return L"Пауза";
         case scraper::state_paused:
            return L"Продожить";
         }

         assert(false);
         return L"";
      }

      wstring status_text ( scraper::state_t state )
      {
         switch (state)
         {
         case scraper::state_stopped:
            return L"Остановлено";
         case scraper::state_started:
            return L"Поиск";
         case scraper::state_paused:
            return L"Пауза";
         }

         assert(false);
         return L"";
      }
   }

   main_window_t::main_window_t ()
      : scraper_ ( this )
      , offers_founded_ ( 0u )
      , offers_processed_ ( 0u )
      , send_widget_ ( this )
   {
      ui_.setupUi(this);

      QSortFilterProxyModel * proxy = new QSortFilterProxyModel(ui_.table);
      proxy->setSourceModel(new impl::offers_table_model_t(proxy));

      ui_.table->setModel(proxy);

      QObject::connect(&scraper_, SIGNAL(started()),
                       this, SLOT(on_status_changed()));
      QObject::connect(&scraper_, SIGNAL(finished(bool)),
                       this, SLOT(on_status_changed()));

      QObject::connect(&scraper_, SIGNAL(paused(bool)),
                       this, SLOT(on_status_changed()));
      QObject::connect(&scraper_, SIGNAL(resumed()),
                       this, SLOT(on_status_changed()));

      QObject::connect(&scraper_, SIGNAL(offer_processed(scraper::offer_t const&)),
                       this, SLOT(on_offer_processed(scraper::offer_t const&)));
      QObject::connect(&scraper_, SIGNAL(offers_founded(unsigned)),
                       this, SLOT(on_offers_founded(unsigned)));

      QObject::connect(ui_.table->selectionModel(), SIGNAL(selectionChanged(QItemSelection const&, QItemSelection const&)),
                       this, SLOT(on_selection_changed()));

      update_ui();
   }

   vector<scraper::offer_t const*>
      main_window_t::get_selected_offers () const
   {
      QModelIndexList const selected =
         ui_.table->selectionModel()->selectedRows();

      vector<scraper::offer_t const*> offers;
      for (int i = 0, count = selected.size(); i != count; ++i)
      {
         int const row =
            proxy()->mapToSource(selected[i]).row();
         offers.push_back(&model()->get_offer(row));
      }
      return offers;
   }

   // gui slots
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::on_searching_start ()
   {
      switch (scraper_.state())
      {
      case scraper::state_stopped:
         scraper_.start();
         break;

      case scraper::state_started:
         scraper_.pause();
         break;

      case scraper::state_paused:
         scraper_.resume();
         break;
      }
   }

   void main_window_t::on_searching_stop ()
   {
      scraper_.stop();
   }

   void main_window_t::on_clear ()
   {
      model()->rem_offers();
      update_ui();
   }

   void main_window_t::on_send ()
   {
      send_widget_.show();
   }

   void main_window_t::on_show_offer ( QModelIndex const& index )
   {
      scraper::offer_t const& offer =
         model()->get_offer(proxy()->mapToSource(index).row());

      QWebView * view = new QWebView();
      view->setWindowTitle(utils::to_qt(offer.model));
      view->setWindowIcon(QIcon(":/icons/icons/web.ico"));

      view->setWindowModality(Qt::ApplicationModal);
      view->setAttribute(Qt::WA_DeleteOnClose, true);

      view->load(offer.url);
      view->show();
   }

   void main_window_t::on_selection_changed ()
   {
      ui_.send->
         setEnabled(!ui_.table->selectionModel()->selectedRows().isEmpty());
   }

   // scraper slots
   //////////////////////////////////////////////////////////////////////////
   void main_window_t::on_status_changed ()
   {
      if (scraper_.state() == scraper::state_stopped)
      {
         offers_processed_ = 0u;
         offers_founded_ = 0u;
      }

      update_ui();
   }

   void main_window_t::on_offer_processed ( scraper::offer_t const& offer )
   {
      ui_.table->setSortingEnabled(false);
      model()->add_offer(offer);
      ui_.table->setSortingEnabled(true);

      ++offers_processed_;
      update_ui();
   }

   void main_window_t::on_offers_founded ( unsigned count )
   {
      offers_founded_ = count;
      update_ui();
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

   void main_window_t::update_ui ()
   {
      scraper::state_t const state =
         scraper_.state();

      wformat status_fmt (L"[%s] %s");
      status_fmt % status_text(state);

      int progress = 0;
      switch (state)
      {
      case scraper::state_stopped:
         {
            static wformat msg_fmt (L"Объявлений найдено: %d");
            status_fmt % str(msg_fmt % model()->offers_count());
         }
         break;

      case scraper::state_started:
      case scraper::state_paused:
         {
            static wformat msg_fmt (L"Объявлений обработано: %d/%d");
            status_fmt % str(msg_fmt % offers_processed_ % offers_founded_);

            if (offers_founded_ != 0)
               progress =
                  static_cast<int>(100. * offers_processed_ / offers_founded_);
         }
         break;
      }

      ui_.status->showMessage(utils::to_qt(str(status_fmt)));
      ui_.progress->setValue(progress);

      ui_.searching_start->
         setText(utils::to_qt(statrt_button_text(state)));
      ui_.searching_stop->setEnabled
         (state != scraper::state_stopped);

      ui_.clear->setEnabled(model()->offers_count() > 0u);
  }
}