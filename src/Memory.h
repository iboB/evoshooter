//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// some basic additional functions concerning memory management
#pragma once

template <typename T>
void safe_delete(T*& t)
{
    delete t;
    t = nullptr;
}

#define OFFSET(i) ((char *)NULL + (i))

#if !defined(_MSC_VER)
#    define _countof(x) (sizeof(x)/sizeof(x[0]));
#endif

// the size of the data within a std::vector
template <typename T>
size_t data_size(const std::vector<T>& vec)
{
    return sizeof(T)*vec.size();
}

// dynamic cast on debug builds to catch wrong casts
template <typename Target, typename Source>
Target debug_cast(Source s)
{
#if defined(NDEBUG)
    return static_cast<Target>(s);
#else
    return dynamic_cast<Target>(s);
#endif
}

// fill memory with zero
inline void zero_memory(void* mem, size_t size)
{
    ::memset(mem, 0, size);
}
