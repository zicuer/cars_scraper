#pragma once

namespace scraper
{
   struct phone_number_t
   {
      wstring area_code;
      wstring number;
   };

   typedef
      vector<phone_number_t>
      phone_numbers_t;

   struct offer_t
   {
      QUrl url;

      wstring  model;
      optional<unsigned> year;
      optional<unsigned> run;
      optional<unsigned> price;

      phone_numbers_t
         phone_numbers;
   };

   optional<wstring>  get_offer_id ( QUrl const& url );
   optional<unsigned> get_year  ( QString const& str );
   optional<unsigned> get_run   ( QString const& str );
   optional<unsigned> get_price ( QString const& str );
   optional<phone_number_t>
                      get_phone_number ( QString const& str );
}