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
#include "InGameState.h"
#include "GUILayer.h"
#include "Effect.h"
#include "Texture.h"
#include "GLSentries.h"
#include "Application.h"

using namespace mathgp;

InGameState::InGameState(const char* texture, int time)
{
    m_textureName = texture;
    m_time = time;
}

void InGameState::initialize()
{
    m_effect = new Effect;

    m_effect->loadVertexShaderFromFile("shaders/simple.vert");
    m_effect->loadPixelShaderFromFile("shaders/simple.frag");

    m_effect->link();

    m_texture = new Texture;
    m_texture->loadFromFile(m_textureName.c_str());
}

void InGameState::deinitialize()
{
    safe_delete(m_effect);
}

void InGameState::handleEvent(const SDL_Event& event)
{
}

void InGameState::update(int dt)
{
    m_time -= dt;
    if (m_time <= 0)
    {
        //if (m_prevState)
        {
            Application::instance().experiMENTAL();
        }
        //else
        //{
        //    if (!m_nextState)
        //    {
        //        std::shared_ptr<GameState> ptr(new InGameState("overlay/game_logo.png", 2000));
        //        pushState(ptr);
        //    }
        //}        
    }
}

void InGameState::draw()
{
    SENTRY(GLDisableSentry, GL_CULL_FACE);
    SENTRY(GLDisableSentry, GL_DEPTH_TEST);
    SENTRY(GLEnableSentry, GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    struct Vertex
    {
        vector3 pos;
        vector2 uv;
    };

    Vertex quad[] =
    {
        { vc(0, 0, 0.4f), vc(0, 1) },
        { vc(1, 0, 0.4f), vc(1, 1) },
        { vc(0, 1, 0.4f), vc(0, 0) },
        { vc(1, 1, 0.4f), vc(1, 0) },
    };

    unsigned indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    const int Attr_Pos = 0;
    const int Attr_UV = 1;

    m_effect->use();
    m_effect->bindCustomAttribute("inPos", Attr_Pos);
    m_effect->bindCustomAttribute("inTexCoord", Attr_UV);

    auto projection = matrix::ortho_rh(0, 1, 0, 1, 1, 100);
    
    int pvm = m_effect->getParameterByName("pvm");
    m_effect->setParameter(pvm, projection);

    int tex = m_effect->getParameterByName("colorMap");
    m_effect->setParameter(tex, *m_texture);
    
    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->pos);
    glEnableVertexAttribArray(Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->uv);
    glEnableVertexAttribArray(Attr_UV);

    glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, indices);

    glDisableVertexAttribArray(Attr_Pos);
    glDisableVertexAttribArray(Attr_UV);
}


