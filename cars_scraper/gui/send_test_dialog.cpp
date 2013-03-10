#include "stdafx.h"

#include "send_test_dialog.h"
#include "send_test_dialog_moc.h"

namespace gui
{
   send_test_dialog_t::send_test_dialog_t ( QWidget * parent )
      : QDialog ( parent )
   {
      ui_.setupUi(this);
   }

   // QDialog slots
   //////////////////////////////////////////////////////////////////////////
   void send_test_dialog_t::accept ()
   {
      sender::abonents_t abonents;
      abonents.insert(utils::from_qt(ui_.number->text()));

      wstring const msg =
         utils::from_qt(ui_.msg->toPlainText());

      sender_.send(abonents, msg, true);
      QDialog::accept();
   }
}