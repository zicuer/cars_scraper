#pragma once

#include "impl/page.h"
#include "impl/request.h"
#include "offer.h"

namespace scraper
{
   enum state_t
   {
      state_stopped,
      state_started,
      state_paused
   };

   class scraper_t
      : public QObject
   {
   public:
      explicit scraper_t ( QObject * parent = NULL );

      state_t state () const;

   public slots:
      void start ();
      void stop ();

      void pause ();
      void resume ();

   private slots:
      void on_loaded ( bool ok );
      void on_timeout ();

   private:
      void stop_current_request ( bool processed );
      void process_next_request ();

      bool check_captcha ();

      bool process_offers_request ();
      bool process_offer_request  ();
      bool process_phones_request ();

   signals:
      void started ();
      void finished ( bool user );

      void paused ( bool user );
      void resumed ();

      void offer_processed ( scraper::offer_t const& offer );
      void offers_founded ( unsigned count );

   private:
      impl::page_t * page_;

      impl::requests_queue_t requests_queue_;
      optional<impl::request_t> request_;

   private:
      Q_OBJECT
   };
}