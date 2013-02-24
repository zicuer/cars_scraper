#include "stdafx.h"
#include "utils.h"

namespace utils
{
   // qt utils
   //////////////////////////////////////////////////////////////////////////
   QString to_qt ( wstring const& str )
   {
      return QString::fromStdWString(str);
   }

   wstring from_qt ( QString const& str )
   {
      return str.toStdWString();
   }

   QUrl to_url ( QString const& url )
   {
      return QUrl(QUrl::fromPercentEncoding(QByteArray().append(url)));
   }
}