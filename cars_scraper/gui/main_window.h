#pragma once

#include "main_window_ui.h"
#include "scraper/scraper.h"

namespace gui
{
   class main_window_t
      : public QMainWindow
   {
   public:
      main_window_t ();

   // gui
   private slots:
      void on_searching_start ();
      void on_searching_stop  ();
      void on_clear ();

      void on_show_offer ( int row, int column );

   // scraper
   private slots:
      void on_started  ();
      void on_finished ( bool canceled );

      void on_offer_processed ( scraper::offer_t const& offer );
      void on_offers_founded ( unsigned count );

   private:
      void update_progress ();

   private:
      typedef
         map<wstring, scraper::offer_t>
         offers_t;

      scraper::scraper_t scraper_;
      offers_t offers_;

      unsigned offers_founded_;
      unsigned offers_processed_;

   private:
      Ui::main_window ui_;

   private:
      Q_OBJECT
   };
}