//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
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
