#pragma once

// STD
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <functional>
#include <random>
#include <streambuf>
#include <time.h>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>

// LOKI
#pragma warning(push)
#pragma warning(disable:4702)
#include <loki/Singleton.h>
#pragma warning(pop)

// LOCAL
#include "util/options.h"
#include "util/cypher_definition.h"
#include "util/cypher_parameters.h"
#include "util/parsing.h"
#include "util/stdutil.h"
#include "cypher_manager.h"

// Windows
#include <Windows.h>
#include <Shellapi.h>

static const char * APP_VERSION = "0.8.2";