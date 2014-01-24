//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// version and application info
#pragma once

#define APP_NAME "gd-proto"

#define APP_VERSION_MAJOR 0
#define APP_VERSION_MINOR 1
#define APP_VERSION_BUILD 0

#define _INTERNAL_STRINGIZE(x) #x
#define STRINGIZE(x) _INTERNAL_STRINGIZE(x)
#define APP_VERSION_STRING STRINGIZE(APP_VERSION_MAJOR.APP_VERSION_MINOR.APP_VERSION_BUILD)
