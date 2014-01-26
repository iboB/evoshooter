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
#include "Hud.h"

using namespace mathgp;

Hud::Hud()
{
    int indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    memcpy(m_indices, indices, sizeof(indices));

    float hh = 0.1f;
    float top = 1;
    float bot = 1 - hh;

    {
        TexQuad quad =
        {
            {
                { vc(0,  bot, 0), vc(0, 1) },
                { vc(hh, bot, 0), vc(1, 1) },
                { vc(0,  top, 0), vc(0, 0) },
                { vc(hh, top, 0), vc(1, 0) },
            }
        };

        m_texQuads.push_back(quad);
    }
}

Hud::~Hud()
{

}

void Hud::update()
{

}

void Hud::draw()
{

}