#pragma once

namespace scraper {
namespace impl
{
   enum request_type
   {
      rt_offers,
      rt_offer,
      rt_phone_numbers
   };

   struct request_t
   {
      request_t ( QUrl const& url,
                  request_type type );

      template<class T>
         void set_data ( T const& data );
      template<class T>
         T const* data () const;
      template<class T>
         T * data ();

      QUrl url;
      request_type type;

      unsigned captchas_count;
      unsigned timeouts_count;

   private:
      any data_;
   };

   typedef
      list<request_t>
      requests_queue_t;

   // impl
   //////////////////////////////////////////////////////////////////////////
   template<class T>
      void request_t::set_data ( T const& data )
   {
      data_ = data;
   }

   template<class T>
      T const* request_t::data () const
   {
      try
      {
         return any_cast<T>(&data_);
      }
      catch (bad_any_cast const&)
      {}
      return NULL;
   }

   template<class T>
      T * request_t::data ()
   {
      try
      {
         return any_cast<T>(&data_);
      }
      catch (bad_any_cast const&)
      {}
      return NULL;
   }
}}