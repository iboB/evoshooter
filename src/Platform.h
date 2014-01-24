//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// compiler and platform specific checks and definitions
#pragma once

////////////////////////////////////////////////////////
// compiler specific stuff

#if defined(_MSC_VER)
#    if _MSC_VER < 1600
#        error "A minimum cl version of 16 is required to compile gd-proto"
#    endif
#elif defined(__GNUC__)
#    define GCC_VERSION (__GNUC__ * 10000 \
        + __GNUC_MINOR__ * 100 \
        + __GNUC_PATCHLEVEL__)
    
#    if GCC_VERSION < 40502
#        error "A minimum gcc version of 4.5.2 is required to compile gd-proto"
#    endif
 
#    if GCC_VERSION < 40600
        // can be emulated through
        // http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr#Solution_and_Sample_Code
        // but no need to maintain a global variable.
        // the define is fine for now
#        define nullptr NULL
#    endif

#    if GCC_VERSION < 47000
        // no way to emulate its behavior for real, but not a big deal, anyway
#        define override
#    endif
#endif
