#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWebKit/QtWebKit>
#include <QtXml/QtXml>

#include <boost/optional.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/format.hpp>

#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/noncopyable.hpp>

using namespace boost;

#include <string>
#include <vector>
#include <list>
#include <set>
#include <locale>

using namespace std;

#include <assert.h>

#include "utils.h"
#include "settings.h"