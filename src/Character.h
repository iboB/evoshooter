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

#include "AnimationsController.h"

class Character : public Object
{
public:
    Character(const mathgp::vector3& position, const std::string& name);

    virtual void Move(const mathgp::vector3& position);
    virtual void Die();
    virtual void GetDamage();

    virtual void update(int dt);

protected:
    AnimationsController m_AnimationsController;
};
