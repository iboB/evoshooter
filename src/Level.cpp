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
#include "Level.h"

#include "GLExtensions.h"

using namespace mathgp;

Level::Level()
{
    const point3 zero = point3::zero();

    const point3 levelMin = vc(0, 0, 0);
    const point3 levelMax = vc(50, 50, 20);

    point3 pmin, pmax;
    
    Vertex vertices[4 * 4];
    auto vptr = vertices;
    
    // bottom
    pmin = zero;
    pmin.xy() = levelMin.xy();
    pmax = zero;
    pmax.xy() = levelMax.xy();
    createQuad(vertices, pmin, pmax);
    
    // back
    pmin = zero;
    pmin.xy() = levelMin.xy();
    pmax = zero;
    pmax.xy() = levelMax.xy();
    createQuad(vertices, pmin, pmax);
}

Level::~Level()
{

}

void Level::draw()
{

}

void Level::createQuad(Vertex* target, const mathgp::point3& min, const mathgp::point3 max)
{

}