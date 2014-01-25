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
#pragma once

#include "ImplicitSingleton.h"

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;

class RenderManager : public ImplicitSingleton<RenderManager>
{
public:

    void AddSprite(Sprite* sprite);

    void RemoveSprite(Sprite* sprite);

    void Clear();

    void Render(const mathgp::matrix4& viewProj);

private:
    void Sort();

    typedef std::set<Sprite*> Sprites;
    typedef std::vector<Sprite*> SpritesVector;

    Sprites m_RenderList;

    SpritesVector m_SortedSprites;
};