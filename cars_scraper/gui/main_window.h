#pragma once

#include "main_window_ui.h"
#include "send_widget.h"
#include "scraper/scraper.h"

namespace gui
{
   namespace impl
   {
      class offers_table_model_t;
   }

   class main_window_t
      : public QMainWindow
   {
   public:
      main_window_t ();

      vector<scraper::offer_t const*>
         get_selected_offers () const;

   // gui
   private slots:
      void on_searching_start ();
      void on_searching_stop  ();
      void on_clear ();
      void on_send ();

      void on_show_offer ( QModelIndex const& index );
      void on_selection_changed ();

   // scraper
   private slots:
      void on_status_changed ();

      void on_offer_processed ( scraper::offer_t const& offer );
      void on_offers_founded ( unsigned count );

   private:
      QAbstractProxyModel * proxy () const;
      impl::offers_table_model_t * model () const;

      void update_ui ();

   private:
      scraper::scraper_t scraper_;

      unsigned offers_founded_;
      unsigned offers_processed_;

   private:
      Ui::main_window ui_;
      send_widget_t send_widget_;

   private:
      Q_OBJECT
   };
}