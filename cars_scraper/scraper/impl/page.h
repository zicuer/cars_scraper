#pragma once

#include "filter.h"

namespace scraper {
namespace impl
{
   class page_t
      : public QObject
   {
   public:
      page_t ( QObject * parent = NULL,
               filter_t filter = NULL );
      void reset ();

      void load ( QUrl const& url,
                  optional<double> timeout );
      void stop ();

   public:
      void set_visible ( bool visible );
      bool is_visible () const;

      optional<wstring>
         find_any ( wstring const& compressed_query ) const;
      vector<wstring>
         find_all ( wstring const& compressed_query ) const;

   private:
      void set_manager_filter ( filter_t filter );

   signals:
      void loaded ( bool ok );
      void timeout ();
      void canceled ();

   private:
      QWebPage * page_;
      QTimer * timer_;

      filter_t filter_;

   private:
      Q_OBJECT
   };
}}