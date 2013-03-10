#pragma once

#include "settings_dialog_ui.h"

namespace gui
{
   class settings_dialog_t
      : public QDialog
   {
   public:
      explicit settings_dialog_t ( QWidget * parent = NULL );

   // QDialog slots
   public slots:
      void accept();
      void reject();

   private:
      void read_settings ();
      void write_settings ();

   private:
      settings_t settings_;

   private:
      Ui::settings_dialog ui_;

   private:
      Q_OBJECT
   };
}