#pragma once

#include "send_dialog_ui.h"
#include "sender/sender.h"

namespace gui
{
   class main_window_t;

   class send_dialog_t
      : public QDialog
   {
   public:
      explicit send_dialog_t ( main_window_t * main_window );

   private slots:
      void on_msg_changed ();
      void on_send ();
      void on_sended ( unsigned count );

   private:
      sender::sender_t sender_;

   private:
      Ui::send_dialog ui_;
      main_window_t const* main_window_;

   private:
      Q_OBJECT
   };
}