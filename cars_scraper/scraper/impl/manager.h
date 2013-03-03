#pragma once

#include "filter.h"

namespace scraper {
namespace impl
{
   class manager_t
      : public QNetworkAccessManager
   {
   public:
      manager_t ( QObject * parent = NULL );
      void set_filter ( filter_t filter );

   // QNetworkAccessManager
   protected:
      QNetworkReply * createRequest ( Operation op, QNetworkRequest const& request,
                                      QIODevice * outgoingData );

   private:
      filter_t filter_;

   private:
      Q_OBJECT
   };
}}