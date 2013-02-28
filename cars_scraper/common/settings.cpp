#include "stdafx.h"
#include "settings.h"

settings_t::settings_t ()
: settings_ ( utils::to_qt(L"settings.ini"),
              QSettings::IniFormat )
{}
