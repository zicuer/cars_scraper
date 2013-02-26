#pragma once

#include "impl/page.h"
#include "impl/request.h"
#include "offer.h"

namespace scraper
{
   class scraper_t
      : public QObject
   {
   public:
      scraper_t ( QObject * parent = NULL );

   public slots:
      void start ();
      void stop ();

   private slots:
      void on_loaded ( bool ok );
      void on_timeout ();

   private:
      void stop_current_request ( bool processed );
      void process_next_request ();

   signals:
      void started ();
      void finished ( bool canceled );

      void offer_processed ( offer_t const& offer );
      void offers_founded ( unsigned count );

   private:
      impl::page_t * page_;

      impl::requests_queue_t requests_queue_;
      optional<impl::request_t> request_;

   private:
      Q_OBJECT
   };
}