#pragma once

#include "scraper/offer.h"

namespace sender
{
   typedef
      set<wstring>
      abonents_t;

   class sender_t
      : public QObject
   {
   public:
      explicit sender_t ( QObject * parent = NULL );

      void send ( abonents_t const& abonents,
                  wstring const& msg, bool force = false );

   private slots:
      void on_finished ( QNetworkReply * reply );

   signals:
      void sended ( unsigned count );

   private:
      QNetworkAccessManager * manager_;
      set<wstring> sended_;

   private:
      Q_OBJECT
   };
}