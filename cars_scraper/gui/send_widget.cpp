#include "stdafx.h"

#include "send_widget.h"
#include "send_widget_moc.h"

#include "main_window.h"

namespace gui
{
   send_widget_t::send_widget_t ( main_window_t const* main_window )
      : main_window_ ( main_window )
   {
      ui_.setupUi(this);
      QWidget::setWindowModality(Qt::WindowModal);

      QObject::connect(&sender_, SIGNAL(sended(unsigned)),
                       this, SLOT(on_sended(unsigned)));
   }

   // slots
   //////////////////////////////////////////////////////////////////////////
   void send_widget_t::on_msg_changed ()
   {
      ui_.send->setEnabled(!ui_.msg->toPlainText().isEmpty());
   }

   void send_widget_t::on_send ()
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
      QWidget::setEnabled(false);
   }

   void send_widget_t::on_sended ( unsigned count )
   {
      static wformat fmt (L"Сообщений отправлено: %d");

      QMessageBox::information(this, utils::to_qt(L"Информация"),
         utils::to_qt(str(fmt % count)));

      QWidget::setEnabled(true);
      QWidget::close();
   }
}