#include "stdafx.h"

#include "settings_dialog.h"
#include "settings_dialog_moc.h"

namespace gui
{
   settings_dialog_t::settings_dialog_t ( QWidget * parent )
      : QDialog ( parent )
      , send_test_ ( new send_test_dialog_t(this) )
   {
      ui_.setupUi(this);
      read_settings();
   }

   // QDialog slots
   //////////////////////////////////////////////////////////////////////////
   void settings_dialog_t::accept()
   {
      write_settings();
      QDialog::accept();
   }

   void settings_dialog_t::reject()
   {
      read_settings();
      QDialog::reject();
   }

   // slots
   //////////////////////////////////////////////////////////////////////////
   void settings_dialog_t::on_apply ()
   {
      write_settings();
   }

   void settings_dialog_t::on_test ()
   {
      send_test_->open();
   }

   // impl
   //////////////////////////////////////////////////////////////////////////
   void settings_dialog_t::read_settings ()
   {
      ui_.key->setText(settings_.get_value<QString>(L"sender/key"));
      ui_.from->setText(settings_.get_value<QString>(L"sender/from"));

      ui_.search_page->setText(settings_.get_value<QString>(L"scraper/search_page"));
      ui_.timeout->setValue(settings_.get_value<double>(L"scraper/loading_timeout"));
      ui_.max_timeouts_count->setValue(settings_.get_value<int>(L"scraper/max_timeouts_count"));
      ui_.max_captchas_count->setValue(settings_.get_value<int>(L"scraper/max_captchas_count"));
   }

   void settings_dialog_t::write_settings ()
   {
      settings_.set_value(L"sender/key", ui_.key->text());
      settings_.set_value(L"sender/from", ui_.from->text());

      settings_.set_value(L"scraper/search_page", ui_.search_page->text());
      settings_.set_value(L"scraper/loading_timeout", ui_.timeout->value());
      settings_.set_value(L"scraper/max_timeouts_count", ui_.max_timeouts_count->value());
      settings_.set_value(L"scraper/max_captchas_count", ui_.max_captchas_count->value());
   }
}