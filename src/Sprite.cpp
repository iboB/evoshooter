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
#include "RenderManager.h"

unsigned Sprite::sm_Indices[] = 
{
    0, 1, 2,
    2, 1, 3,
};

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
, m_IsRendering(false)
, m_Scale(1.0)
, m_CellWidth(0)
, m_CellHeight(0)
, m_FlipX(false)
{}

Sprite::~Sprite()
{
    if (m_Texture)
    {
        ResourceManager::instance().releaseTexture(m_Texture);
    }
   
    RenderManager::instance().RemoveSprite(this);
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

    m_IsRendering = false;

    setScale(m_Scale);
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

    m_IsRendering = false;

    setScale(m_Scale);
}

void Sprite::startRendering(Uint32 frameOffset)
{
    m_StartTime = SDL_GetTicks();
    m_IsRendering = true;
    m_FramesOffset = frameOffset;

    RenderManager::instance().AddSprite(this);
}

void Sprite::restartRendering(Uint32 frameOffset)
{
    m_StartTime = SDL_GetTicks();
    m_IsRendering = true;

    m_FramesOffset = frameOffset;
}

void Sprite::stopRendering()
{
    m_IsRendering = false;
    RenderManager::instance().RemoveSprite(this);
}

void Sprite::setScale(float scale)
{
    m_Scale = scale;
    m_ScaledFrameWidth = float(m_CellWidth) * scale;
    m_ScaledFrameHeight = float(m_CellHeight) * scale;
}

void Sprite::update(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    Uint32 x(0), y(0);

    getCurrentFrame(x, y);

    float u0 = float(x) / float(m_Texture->width());
    float v0 = float(y) / float(m_Texture->height());

    float u1 = float(x + m_CellWidth) / float(m_Texture->width());
    float v1 = float(y + m_CellHeight) / float(m_Texture->height());

    if (m_FlipX)
    {
        std::swap(u0, u1);
    }

    m_Vertices[0] = { Vec::zero, mathgp::vc(u0, v1) };
    m_Vertices[1] = { mathgp::vc(m_ScaledFrameWidth, 0, 0), mathgp::vc(u1, v1) };
    m_Vertices[2] = { mathgp::vc(0, 0, m_ScaledFrameHeight), mathgp::vc(u0, v0) };
    m_Vertices[3] = { mathgp::vc(m_ScaledFrameWidth, 0, m_ScaledFrameHeight), mathgp::vc(u1, v0) };

    float camAngle = acos(abs(camDir.y()));
    mathgp::matrix camAlign = mathgp::matrix::rotation_x(-camAngle);

    for (auto& vert : m_Vertices)
    {
        vert.position = mathgp::transform_coord(vert.position, camAlign);
        vert.position += position;
    }
}

void Sprite::render(const mathgp::matrix4& viewProj)
{
    if (!m_IsRendering)
        return;

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const int Attr_Pos = 0;
    const int Attr_UV = 1;

    Effect* spriteEffect = ResourceManager::instance().getSpriteEffect();

    spriteEffect->use();
    spriteEffect->bindCustomAttribute("inPos", Attr_Pos);
    spriteEffect->bindCustomAttribute("inTexCoord", Attr_UV);

    int pvm = spriteEffect->getParameterByName("pvm");
    spriteEffect->setParameter(pvm, viewProj);

    int tex = spriteEffect->getParameterByName("colorMap");
    spriteEffect->setParameter(tex, *m_Texture);

    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), &m_Vertices->position);
    glEnableVertexAttribArray(Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), &m_Vertices->texCoord);
    glEnableVertexAttribArray(Attr_UV);

    glDrawElements(GL_TRIANGLES, _countof(sm_Indices), GL_UNSIGNED_INT, sm_Indices);

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
            stopRendering();
            timeDiff = m_Duration - 1;
        }
    }

    m_CurrentFrame = Uint32(float(totalFrames) * (float(timeDiff) / float(m_Duration))) % totalFrames;

    if (m_FramesOffset)
    {
        m_CurrentFrame += m_FramesOffset;
        m_CurrentFrame %= totalFrames;
    }

    x = m_StartX + ((m_CurrentFrame % m_Cols) * m_CellWidth);
    y = m_StartY + ((m_CurrentFrame / m_Cols) * m_CellHeight);
}
