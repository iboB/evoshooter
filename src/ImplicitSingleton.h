//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// ImplicitSingleton provides a static method instance
// the first call of instance will create the object
// usage: crtp: class SomeClass : public ImplicitSingleton<SomeClass>
#pragma once

template <typename T>
class ImplicitSingleton
{
public:
    static T& instance()
    {
        static T theInstance;
        return theInstance;
    }
};
