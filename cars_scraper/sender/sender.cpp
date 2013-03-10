#include "stdafx.h"

#include "sender.h"
#include "sender_moc.h"

namespace sender
{
   sender_t::sender_t ( QObject * parent )
      : QObject ( parent )
      , manager_ ( new QNetworkAccessManager(this) )
   {
      QObject::connect(manager_, SIGNAL(finished(QNetworkReply *)),
                       this, SLOT(on_finished(QNetworkReply *)));
   }

   void sender_t::send ( abonents_t const& abonents,
                         wstring const& msg )
   {
      settings_t settings;

      vector<wstring> sms;
      wstring const from =
         settings.get_value<wstring>(L"sender/from");
      for each (wstring const& abonent in abonents)
      {
         if (sended_.find(abonent) == sended_.end())
         {
            static wformat sms_fmt (L"<sms from=\"%s\" to=\"%s\">%s</sms>");
            sms.push_back(str(sms_fmt % from % abonent % msg));
         }
      }

      static wformat query_fmt (L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                L"<send apikey=\"%s\">\n%s</send>");
      wstring const query =
         str(query_fmt % settings.get_value<wstring>(L"sender/key") %
             algorithm::join(sms, L"\n"));

      manager_->post(QNetworkRequest(utils::to_url(settings.get_value<QString>(L"sender/url"))),
         utils::to_qt(query).toUtf8());
   }

   // slots
   //////////////////////////////////////////////////////////////////////////
   void sender_t::on_finished ( QNetworkReply * reply )
   {
      unsigned successed = 0u;

      QDomDocument document;
      if (document.setContent(reply))
      {
         QDomNodeList const sms =
            document.elementsByTagName(utils::to_qt(L"sms"));
         for (int i = 0, count = sms.count(); i != count; ++i)
         {
            QDomElement element = sms.at(i).toElement();
            if (element.attribute(utils::to_qt(L"status")) == "0")
            {
               ++successed;
               sended_.insert(utils::from_qt(element.attribute(utils::to_qt(L"to"))));
            }
         }
      }

      emit(sended(successed));
      reply->deleteLater();
   }
}