#pragma once

namespace scraper {
namespace impl
{
   typedef
      function<bool (QUrl const&)>
      filter_t;
}}