#include "stdafx.h"

#include "send_dialog.h"
#include "send_dialog_moc.h"

#include "main_window.h"

namespace gui
{
   namespace
   {
      bool is_russian ( wchar_t const ch )
      {
         return ((ch >= L'а' && ch <= L'я') ||
                 (ch >= L'А' && ch <= L'Я'));
      }

      bool is_russian ( wstring const& str )
      {
         for each (wchar_t ch in str)
            if (is_russian(ch))
               return true;
         return false;
      }
   }

   send_dialog_t::send_dialog_t ( main_window_t * main_window )
      : QDialog ( main_window )
      , main_window_ ( main_window )
   {
      ui_.setupUi(this);
      QObject::connect(&sender_, SIGNAL(sended(unsigned)),
                       this, SLOT(on_sended(unsigned)));
   }

   // slots
   //////////////////////////////////////////////////////////////////////////
   void send_dialog_t::on_msg_changed ()
   {
      wstring msg = utils::from_qt(ui_.msg->toPlainText());
      ui_.send->setEnabled(!msg.empty());

      unsigned const cur_length = msg.size();
      unsigned const max_length = is_russian(msg) ? 70u : 140u;

      if (cur_length > max_length)
      {
         ui_.msg->setPlainText(utils::to_qt(msg.substr(0, max_length)));
         QTextCursor cursor = ui_.msg->textCursor();
         cursor.setPosition(max_length);
         ui_.msg->setTextCursor(cursor);
      }
      else
      {
         static wformat letters_fmt (L"%3d");
         ui_.letters->setText(utils::to_qt(str(letters_fmt % (max_length - cur_length))));
      }
   }

   void send_dialog_t::on_send ()
   {
      sender::abonents_t abonents;
      for each (scraper::offer_t const* offer in main_window_->get_selected_offers())
      {
         static wformat abonent_fmt (L"7%s%s");
         for each (scraper::phone_number_t const& phone_number in offer->phone_numbers)
            abonents.insert(str(abonent_fmt % phone_number.area_code %
               phone_number.number));
      }
      sender_.send(abonents, utils::from_qt(ui_.msg->toPlainText()));
      QDialog::setEnabled(false);
   }

   void send_dialog_t::on_sended ( unsigned count )
   {
      static wformat fmt (L"Сообщений отправлено: %d");

      QMessageBox::information(this, utils::to_qt(L"Информация"),
         utils::to_qt(str(fmt % count)));

      QDialog::setEnabled(true);
      QDialog::accept();
   }
}