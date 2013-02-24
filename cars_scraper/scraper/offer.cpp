#include "stdafx.h"
#include "offer.h"

namespace scraper
{
   namespace
   {
      void erase_spaces ( wstring & str )
      {
         static wregex const space_expr (L"\\s+", wregex::perl);
         algorithm::erase_all_regex(str, space_expr);
      }

      optional<unsigned> to_number ( wstring str )
      {
         erase_spaces(str);

         static wregex const number_expr (L"(\\d+)", wregex::perl);
         wsmatch number_match;
         if (regex_search(str, number_match, number_expr))
         {
            try { return lexical_cast<unsigned>(number_match.str(1)); }
            catch (bad_lexical_cast const& ) {}
         }

         return none;
      }
   }

   optional<wstring> get_offer_id ( QUrl const& url )
   {
      static wregex const offer_expr (L"/([\\da-z-]+)\\.html",
         wregex::icase | wregex::perl);

      wstring const url_str =
         utils::from_qt(url.toString());

      wsmatch offer_match;
      if (regex_search(url_str, offer_match, offer_expr))
         return offer_match.str(1);

      return none;
   }

   optional<unsigned> get_year ( QString const& str )
   {
      return to_number(utils::from_qt(str));
   }

   optional<unsigned> get_run ( QString const& str )
   {
      return to_number(utils::from_qt(str));
   }

   optional<unsigned> get_price ( QString const& str )
   {
      return to_number(utils::from_qt(str));
   }

   optional<phone_number_t> get_phone_number ( QString const& str )
   {
      wstring phone_number_str = utils::from_qt(str);

      static wregex const space_expr (L"[\\s-]+");
      algorithm::erase_all_regex(phone_number_str, space_expr);

      static wregex const phone_number_expr (L"(\\+7|8)?\\((\\d{3})\\)(\\d{7})",
                                             wregex::perl);

      wsmatch phone_number_match;
      if (regex_search(phone_number_str, phone_number_match,
                       phone_number_expr))
      {
         phone_number_t phone_number;

         phone_number.area_code = phone_number_match.str(2);
         phone_number.number = phone_number_match.str(3);

         return phone_number;
      }

      return none;
   }
}