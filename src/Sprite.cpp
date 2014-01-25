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

#include "Sprite.h"
#include "Effect.h"
#include "Texture.h"
#include "ResourceManager.h"

Sprite::Sprite()
: m_StartX(0)
, m_StartY(0)
, m_Width(0)
, m_Height(0)
, m_Rows(0)
, m_Cols(0)
, m_Duration(0)
, m_StartTime(0)
, m_CurrentFrame(0)
, m_Loop(false)
, m_IsRunning(false)
, m_CellWidth(0)
, m_CellHeight(0)
{}

Sprite::~Sprite()
{
    if (m_Texture)
    {
        ResourceManager::instance().releaseTexture(m_Texture);
    }
}

void Sprite::init(const std::string& file, Uint32 startX, Uint32 startY, Uint32 width, Uint32 height, Uint32 rows, Uint32 cols, Uint32 duration, bool loop)
{
    m_StartX = startX;
    m_StartY = startY;
    m_Width = width;
    m_Height = height;

    m_Rows = rows;
    m_Cols = cols;
    m_Duration = duration;

    m_Texture = ResourceManager::instance().getTexture(file);
    m_CurrentFrame = 0;
    m_StartTime = 0;

    m_CellWidth = rows != 0 ? width / cols : width;
    m_CellHeight = cols != 0 ? height / rows : height;

    m_Loop = loop;

    m_IsRunning = false;
}

void Sprite::init(TexturePtr texture, Uint32 startX, Uint32 startY, Uint32 width, Uint32 height, Uint32 rows, Uint32 cols, Uint32 duration, bool loop)
{
    m_StartX = startX;
    m_StartY = startY;
    m_Width = width;
    m_Height = height;

    m_Rows = rows;
    m_Cols = cols;
    m_Duration = duration;

    m_Texture = texture;
    m_CurrentFrame = 0;
    m_StartTime = 0;

    m_CellWidth = rows != 0 ? width / cols : width;
    m_CellHeight = cols != 0 ? height / rows : height;

    m_Loop = loop;

    m_IsRunning = false;
}

void Sprite::startAnimation()
{
    m_StartTime = SDL_GetTicks();
    m_IsRunning = true;
}

void Sprite::restartAnimation()
{
    m_StartTime = SDL_GetTicks();
    m_IsRunning = true;
}

void Sprite::stopAnimation()
{
    m_IsRunning = false;
}

void Sprite::update()
{

}

void Sprite::render()
{
    if (!m_IsRunning)
        return;

    Uint32 x(0), y(0);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    getCurrentFrame(x, y);

    float u0 = float(x) / float(m_Texture->width());
    float v0 = float(y) / float(m_Texture->height());

    float u1 = float(x + m_CellWidth) / float(m_Texture->width());
    float v1 = float(y + m_CellHeight) / float(m_Texture->height());

    struct Vertex
    {
        mathgp::vector3 pos;
        mathgp::vector2 uv;
    };

    Vertex quad[] =
    {
        { mathgp::vc(0, 0, 0.4f), mathgp::vc(u0, v1) },
        { mathgp::vc(5, 0, 0.4f), mathgp::vc(u1, v1) },
        { mathgp::vc(0, 5, 0.4f), mathgp::vc(u0, v0) },
        { mathgp::vc(5, 5, 0.4f), mathgp::vc(u1, v0) },
    };

    /*for (auto& v : quad)
    {
        v.pos.xy() += vc(30, 30);
    }
*/
    unsigned indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    const int Attr_Pos = 0;
    const int Attr_UV = 1;

    Effect* spriteEffect = ResourceManager::instance().getSpriteEffect();

    spriteEffect->use();
    spriteEffect->bindCustomAttribute("inPos", Attr_Pos);
    spriteEffect->bindCustomAttribute("inTexCoord", Attr_UV);

    auto projection = mathgp::matrix::ortho_lh(20, 12, 1, 100);

    int pvm = spriteEffect->getParameterByName("pvm");
    spriteEffect->setParameter(pvm, projection);

    int tex = spriteEffect->getParameterByName("colorMap");
    spriteEffect->setParameter(tex, *m_Texture);

    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->pos);
    glEnableVertexAttribArray(Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->uv);
    glEnableVertexAttribArray(Attr_UV);

    glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, indices);

    glDisableVertexAttribArray(Attr_Pos);
    glDisableVertexAttribArray(Attr_UV);
}

void Sprite::getCurrentFrame(Uint32& x, Uint32& y)
{
    int frame = 0;
    const Uint32 totalFrames = m_Rows * m_Cols;
    if (totalFrames <= 1 || m_Duration == 0)
    {
        x = m_StartX;
        y = m_StartY;
        return;
    }

    Uint32 ticks = SDL_GetTicks();

    Uint32 timeDiff = ticks - m_StartTime;

    if (timeDiff > m_Duration)
    {
        if (m_Loop)
        {
            timeDiff %= m_Duration;
        }
        else
        {
            timeDiff = m_Duration;
        }
    }

    m_CurrentFrame = Uint32(float(totalFrames) * (float(timeDiff) / float(m_Duration))) % totalFrames;

    x = m_StartX + ((m_CurrentFrame % m_Cols) * m_CellWidth);
    y = m_StartY + ((m_CurrentFrame / m_Cols) * m_CellHeight);
}
