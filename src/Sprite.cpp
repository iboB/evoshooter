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

    m_CellWidth = rows != 0 ? width / rows : 0;
    m_CellHeight = cols != 0 ? height / cols : 0;

    m_Loop = loop;

    m_IsRunning = false;
}

void Sprite::init(TexturePtr texture, Uint32 startX, Uint32 startY, Uint32 width, Uint32 height, Uint32 rows, Uint32 cols, Uint32 duration, bool loop = true)
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

    m_CellWidth = rows != 0 ? width / rows : 0;
    m_CellHeight = cols != 0 ? height / cols : 0;

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

    getCurrentFrame(x, y);
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
