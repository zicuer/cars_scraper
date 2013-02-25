#pragma once

#include "filter.h"

namespace scraper {
namespace impl
{
   class page_t
      : public QObject
   {
   public:
      page_t ( QObject * parent = NULL );
      void reset ();

      void load ( QUrl const& url,
                  optional<double> timeout );
      void stop ();

   public:
      void set_visible ( bool visible );
      bool is_visible () const;

      void set_filter ( filter_t filter );
      filter_t get_filter () const;

      QWebFrame const* frame () const;

   signals:
      void loaded ( bool ok );
      void timeout ();
      void canceled ();

   private:
      QWebPage * page_;
      QTimer * timer_;

   private:
      Q_OBJECT
   };
}}