#pragma once

namespace scraper
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
         T & data ();

      QUrl const url;
      request_type const type;

      unsigned captchas_count_;
      unsigned timeouts_count_;

   private:
      any data_;
   };

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
      T & request_t::data ()
   {
      try
      {
         return any_cast<T>(&data_);
      }
      catch (bad_any_cast const&)
      {}
      return NULL;
   }
}