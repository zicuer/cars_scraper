#pragma once

#include "send_widget_ui.h"
#include "sender/sender.h"

namespace gui
{
   class main_window_t;

   class send_widget_t
      : public QWidget
   {
   public:
      explicit send_widget_t ( main_window_t const* main_window );

   private slots:
      void on_msg_changed ();
      void on_send ();
      void on_sended ( unsigned count );

   private:
      sender::sender_t sender_;

   private:
      Ui::send_widget ui_;
      main_window_t const* main_window_;

   private:
      Q_OBJECT
   };
}