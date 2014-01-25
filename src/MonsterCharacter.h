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

#include "Character.h"

class MonsterCharacter : public Character
{
public:
    MonsterCharacter(const mathgp::vector3& position, const std::string& name);

    void SetMoveDirection(const mathgp::vector3& dir);
    void SetMoveSpeed(float speed);

    virtual void update(int dt);

private:
    mathgp::vector3 m_MoveDirection;
    float m_Speed;// m/s
};
