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

#include "Object.h"

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;

class StaticObject : public Object
{
public:
    StaticObject(SpritePtr sprite, const mathgp::vector3& position, float bc);
    ~StaticObject();

    virtual void update(int dt);

private:
    SpritePtr m_Sprite;
};
