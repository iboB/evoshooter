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
#include "Overlay.h"
#include "Effect.h"
#include "Texture.h"
#include "GLSentries.h"
#include "Application.h"
#include "MainWindow.h"

using namespace mathgp;

Overlay::Overlay()
{
    m_effect = new Effect;

    m_effect->loadVertexShaderFromFile("overlay/overlay.vert");
    m_effect->loadPixelShaderFromFile("overlay/overlay.frag");

    m_effect->link();

    m_texParam = m_effect->getParameterByName("colorMap");
    m_scaleParam = m_effect->getParameterByName("scale");

    m_texture = new Texture;
    m_texture->loadFromFile("overlay/gradient_black_screen_01.png");
    m_texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Overlay::~Overlay()
{
    delete m_effect;
    delete m_texture;
}

void Overlay::draw()
{
    SENTRY(GLDisableSentry, GL_DEPTH_TEST);
    SENTRY(GLEnableSentry, GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    struct Vertex
    {
        vector3 pos;
        vector2 uv;
    };

    static Vertex quad[] =
    {
        { vc(-1, -1, 0.f), vc(0, 0) },
        { vc(1, -1, 0.f), vc(0, 1) },
        { vc(-1, 1, 0.f), vc(1, 0) },
        { vc(1, 1, 0.f), vc(1, 1) },
    };

    static unsigned indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    const int Attr_Pos = 0;
    const int Attr_UV = 1;

    m_effect->use();
    m_effect->bindCustomAttribute("inPos", Attr_Pos);
    m_effect->bindCustomAttribute("inTexCoord", Attr_UV);

    m_effect->setParameter(m_texParam, *m_texture);
    
    vector2 scale;
    auto size = Application::instance().mainWindow()->clientAreaSize();

    scale.y() = 1;// float(size.x()) / size.y();
    scale.x() = 1;
    //scale *= 3;
    m_effect->setParameter(m_scaleParam, scale);

    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->pos);
    SENTRY(GLEnableAttribSentry, Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->uv);
    SENTRY(GLEnableAttribSentry, Attr_UV);

    glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, indices);
}
