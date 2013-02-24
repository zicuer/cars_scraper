#pragma once

namespace utils
{
   // qt utils
   //////////////////////////////////////////////////////////////////////////
   QString to_qt ( wstring const& str );
   wstring from_qt ( QString const& str );
   QUrl to_url ( QString const& url );
}