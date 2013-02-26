#include "stdafx.h"
#include "request.h"

namespace scraper {
namespace impl
{
   request_t::request_t ( QUrl const& url,
                          request_type type )
      : url ( url )
      , type ( type )
      , captchas_count ( 0u )
      , timeouts_count ( 0u )
   {}
}}