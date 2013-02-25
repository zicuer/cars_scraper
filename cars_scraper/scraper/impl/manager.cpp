#include "stdafx.h"
#include "manager.h"

namespace scraper {
namespace impl
{
   manager_t::manager_t ( QObject * parent )
      : QNetworkAccessManager ( parent )
   {}

   void manager_t::set_filter ( filter_t filter )
   {
      filter_ = filter;
   }

   filter_t manager_t::get_filter () const
   {
      return filter_;
   }

   // QNetworkAccessManager
   //////////////////////////////////////////////////////////////////////////
   QNetworkReply * manager_t::createRequest ( Operation op, QNetworkRequest const& request,
                                              QIODevice * outgoingData )
   {
      if (filter_ && !filter_(request.url()))
         return QNetworkAccessManager::createRequest(QNetworkAccessManager::GetOperation,
                                                     QNetworkRequest());
      return QNetworkAccessManager::createRequest(op, request, outgoingData);
   }
}}