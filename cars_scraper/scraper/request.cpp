#include "stdafx.h"
#include "request.h"

namespace scraper
{
   request_t::request_t ( QUrl const& url,
                          request_type type )
      : url ( url )
      , type ( type )
      , captchas_count_ ( 0u )
      , timeouts_count_ ( 0u )
   {}
}