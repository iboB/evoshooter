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

void RenderManager::CLear()
{
    m_RenderList.clear();
}

void RenderManager::Sort()
{

}

void RenderManager::Render(const mathgp::matrix4& viewProj)
{
    for (Sprites::iterator it = m_RenderList.begin(); it != m_RenderList.end(); ++it)
    {
       (*it)->render(viewProj);
    }
}
