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

#include "Effect.h"
#include "Texture.h"
#include "GLSentries.h"

#include "World.h"
#include "MainCharacter.h"

using namespace mathgp;

float hh = 6;
size_t Healtbar_Width = 7;
float Bar_Padding = 0.6f;

size_t Health_Quad_Index;

Hud::Hud()
{
    int indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    memcpy(m_indices, indices, sizeof(indices));

    float top = hh;
    float bot = 0;

    {
        TexQuad quad =
        {
            {
                { vc(0,  bot, 2), vc(0, 1) },
                { vc(hh, bot, 2), vc(1, 1) },
                { vc(0, top, 2), vc(0, 0) },
                { vc(hh, top, 2), vc(1, 0) },
            }
        };

        quad.texture = new Texture;
        quad.texture->loadFromFile("gui/sprites/char_portrait.png");

        m_texQuads.push_back(quad);
    }

    {
        ColorQuad quad =
        {
            {
                { vc(0, bot, 2), vc(0.16f, 0.16f, 0.16f, 1) },
                { vc(hh, bot, 2), vc(0.16f, 0.16f, 0.16f, 1) },
                { vc(0, top, 2), vc(0.16f, 0.16f, 0.16f, 1) },
                { vc(hh, top, 2), vc(0.16f, 0.16f, 0.16f, 1) },
            }
        };

        for (auto& v : quad.vertices)
        {
            v.position.x() += hh;
        }

        quad.vertices[1].position.x() += (Healtbar_Width-1) * hh;
        quad.vertices[3].position.x() += (Healtbar_Width-1) * hh;

        m_colorQuads.push_back(quad);
    }

    {
        ColorQuad quad =
        {
            {
                { vc(0, bot, 2), vc(1, 0, 0, 1) },
                { vc(hh, bot, 2), vc(1, 0, 0, 1) },
                { vc(0, top, 2), vc(1, 0, 0, 1) },
                { vc(hh, top, 2), vc(1, 0, 0, 1) },
            }
        };

        for (auto& v : quad.vertices)
        {
            v.position.x() += hh;
        }

        quad.vertices[1].position.x() += (Healtbar_Width - 1) * hh;
        quad.vertices[3].position.x() += (Healtbar_Width - 1) * hh;

        quad.vertices[0].position.y() += Bar_Padding;
        quad.vertices[1].position.y() += Bar_Padding;
        quad.vertices[2].position.y() -= Bar_Padding;
        quad.vertices[3].position.y() -= Bar_Padding;

        Health_Quad_Index = m_colorQuads.size();
        m_colorQuads.push_back(quad);        
    }



    /////////////////////////////////////////
    m_texturize = new Effect;

    m_texturize->loadVertexShaderFromFile("shaders/simple.vert");
    m_texturize->loadPixelShaderFromFile("shaders/simple.frag");

    m_texturize->link();

    m_texPVMParam = m_texturize->getParameterByName("pvm");
    m_texParam= m_texturize->getParameterByName("colorMap");

    /////////////////////////////////////////
    m_colorize = new Effect;

    m_colorize->loadVertexShaderFromFile("shaders/color.vert");
    m_colorize->loadPixelShaderFromFile("shaders/color.frag");

    m_colorize->link();

    m_colorPVMParam = m_colorize->getParameterByName("pvm");
}

Hud::~Hud()
{
    safe_delete(m_texturize);
    for (auto& quad : m_texQuads)
    {
        delete quad.texture;
    }
}

void Hud::update()
{
    auto& quad = m_colorQuads[Health_Quad_Index];

    int hp = World::instance().mainCharacter()->hp();
    int maxHp = World::instance().mainCharacter()->maxHp();

    float percentHp = float(hp) / maxHp;

    quad.vertices[1].position.x() = quad.vertices[0].position.x() + Healtbar_Width * hh * percentHp;
    quad.vertices[3].position.x() = quad.vertices[2].position.x() + Healtbar_Width * hh * percentHp;
}

void Hud::draw()
{
    enum Attr
    {
        Attr_Pos, Attr_2
    };

    SENTRY(GLDisableSentry, GL_DEPTH_TEST);
    SENTRY(GLDisableSentry, GL_CULL_FACE);
    SENTRY(GLEnableAttribSentry, Attr_Pos);
    SENTRY(GLEnableAttribSentry, Attr_2);

    matrix proj = matrix::ortho_rh(0, 100, 0, 100 * (Screen::size.y() / Screen::size.x()), 1, 10);


    /////////////////////////////////////////

    m_colorize->use();
    m_colorize->bindCustomAttribute("inPos", Attr_Pos);
    m_colorize->bindCustomAttribute("inColor", Attr_2);

    m_colorize->setParameter(m_colorPVMParam, proj);

    for (const auto& quad : m_colorQuads)
    {
        glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), &quad);
        glVertexAttribPointer(Attr_2, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), reinterpret_cast<const char*>(&quad) + sizeof(vector3));

        glDrawElements(GL_TRIANGLES, _countof(m_indices), GL_UNSIGNED_INT, m_indices);
    }

    /////////////////////////////////////////

    m_texturize->use();
    m_texturize->bindCustomAttribute("inPos", Attr_Pos);
    m_texturize->bindCustomAttribute("inTexCoord", Attr_2);

    m_texturize->setParameter(m_texPVMParam, proj);

    for (const auto& quad : m_texQuads)
    {
        m_texturize->setParameter(m_texParam, *quad.texture);

        glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(TexVertex), &quad);

        glVertexAttribPointer(Attr_2, 2, GL_FLOAT, GL_FALSE, sizeof(TexVertex), reinterpret_cast<const char*>(&quad) + sizeof(vector3));

        glDrawElements(GL_TRIANGLES, _countof(m_indices), GL_UNSIGNED_INT, m_indices);
    }
}