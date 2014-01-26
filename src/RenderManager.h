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