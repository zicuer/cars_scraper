#include "stdafx.h"
#include "settings.h"

settings_t::settings_t ()
   : settings_ ( utils::to_qt(L"cars scraper"),
                 utils::to_qt(L"cars scraper") )
{}
