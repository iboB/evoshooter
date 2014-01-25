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

#include "RenderManager.h"
#include "Sprite.h"


void RenderManager::AddSprite(Sprite* sprite)
{
    m_RenderList.insert(sprite);
}

void RenderManager::RemoveSprite(Sprite* sprite)
{
    m_RenderList.erase(sprite);
}

void RenderManager::Clear()
{
    m_RenderList.clear();
    m_SortedSprites.clear();
}

void RenderManager::Sort()
{
    m_SortedSprites.assign(m_RenderList.begin(), m_RenderList.end());

    struct SortComp
    {
        bool operator()(const Sprite* l, const Sprite* r)
        {
            if (l->lastPosition().z() == r->lastPosition().z())
            {
                return l->lastPosition().y() > r->lastPosition().y();
            }

            return l->lastPosition().z() < r->lastPosition().z();
        }
    };

    std::sort(m_SortedSprites.begin(), m_SortedSprites.end(), SortComp());
}

void RenderManager::Render(const mathgp::matrix4& viewProj)
{
    Sort();

    for (SpritesVector::iterator it = m_SortedSprites.begin(); it != m_SortedSprites.end(); ++it)
    {
       (*it)->render(viewProj);
    }

    m_SortedSprites.clear();
}
