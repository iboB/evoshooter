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
// Util.h - some utility functions
#pragma once

namespace Util
{
	// loads and entire file to a new char[]
	const char* LoadFile(const char* filename, size_t* outFileSize = nullptr);

	// random float between 0 and 1
	float Rnd01();
	// random float between -1 and 1
	float Rnd11();

	// returns false if there are no real roots
	bool SolveQuadraticEquation(float a, float b, float c, float& outX1, float& outX2);

    template <typename FwdIter>
    size_t FindMaxIndex(FwdIter begin, FwdIter end)
    {
        size_t index = 0;
        size_t bestIndex = 0;
        FwdIter found = begin;
        ++begin;

        while (++begin != end)
        {
            ++index;

            if (*begin > *found)
            {
                found = begin;
                bestIndex = index;
            }
        }

        return bestIndex;
    }
}

namespace mathgp
{
    // should be there anyway
    inline float clamp(float v, float min, float max)
    {
        if (v < min) return min;
        if (v > max) return max;
        return v;
    }
}