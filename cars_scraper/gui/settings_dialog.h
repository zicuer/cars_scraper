#pragma once

#include "settings_dialog_ui.h"
#include "send_test_dialog.h"

namespace gui
{
   class settings_dialog_t
      : public QDialog
   {
   public:
      explicit settings_dialog_t ( QWidget * parent = NULL );

   // QDialog slots
   public slots:
      void accept ();
      void reject ();

   private slots:
      void on_apply ();
      void on_test ();

   private:
      void read_settings ();
      void write_settings ();

   private:
      settings_t settings_;

   private:
      Ui::settings_dialog ui_;
      send_test_dialog_t * send_test_;

   private:
      Q_OBJECT
   };
}