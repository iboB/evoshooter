//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
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