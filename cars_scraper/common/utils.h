#pragma once

namespace utils
{
   QString to_qt ( wstring const& str );
   wstring from_qt ( QString const& str );
   QUrl to_url ( QString const& url );

   template<class T>
      inline QVariant to_variant ( T const& value );
   template<class T>
      inline T from_variant ( QVariant const& var );

   // impl
   //////////////////////////////////////////////////////////////////////////
   template<class T>
      inline QVariant to_variant ( T const& value )
   {
      return QVariant::fromValue(value);
   }

   template<>
      inline QVariant to_variant ( wstring const& value )
   {
      return QVariant(to_qt(value));
   }

   template<class T>
      inline T from_variant ( QVariant const& var )
   {
      return var.value<T>();
   }

   template<>
      inline wstring from_variant ( QVariant const& var )
   {
      return from_qt(var.value<QString>());
   }
}