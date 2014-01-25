//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// NonCopyable.h - a parent class for non-copyable objects
#pragma once

class NonCopyable
{
	NonCopyable(const NonCopyable&);
public:
	NonCopyable() {}
};
