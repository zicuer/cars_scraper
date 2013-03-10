#pragma once

#include "send_test_dialog_ui.h"
#include "sender/sender.h"

namespace gui
{
   class send_test_dialog_t
      : public QDialog
   {
   public:
      explicit send_test_dialog_t ( QWidget * parent = NULL );

   // QDialog slots
   public slots:
      void accept ();

   private:
      sender::sender_t sender_;

   private:
      Ui::send_test_dialog ui_;

   private:
      Q_OBJECT
   };
}