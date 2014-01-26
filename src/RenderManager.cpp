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
            return l->getSortingY() > r->getSortingY();
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
