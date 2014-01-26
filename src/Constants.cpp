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
#include "EvoShooter.pch.h"
#include "Constants.h"

using namespace mathgp;

namespace Vec
{
    const vector3 up = vector3::coord(0, 0, 1);
    const vector3 zero = vector3::zero();
}

namespace Mat
{
    const mathgp::matrix identity = matrix::identity();
}

namespace Screen
{
    const mathgp::vector2 size = vc(1024, 600);
}