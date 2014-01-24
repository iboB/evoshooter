//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// precompiled header file
#pragma once

// sdl is not situated in a specific folder for msvc
// who knows why...
#if defined(_MSC_VER)
#   include <SDL.h>
#   include <SDL_image.h>
#else 
#   include <SDL/SDL.h>
#endif

#if defined(_WIN32)
#    include <GL/glew.h> //no extensions are available for Windows so use glew
#else
#    define GL_GLEXT_PROTOTYPES
#    include <GL/gl.h>
#    include <GL/glext.h>
#endif

//#define MATHGP_DISABLE_SWIZZLE 
// swizzle still not quite implemented, but enable it for what it's worth
#include <mathgp/mathgp.h>
#include <mathgp/glext.h>
#include <mathgp/stdext.h>

// common std stuff
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <array>
#include <algorithm>

// custom global definitions
#include "Memory.h"
#include "Macros.h"