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

#include "MonsterCharacter.h"

MonsterCharacter::MonsterCharacter(const mathgp::vector3& position, const std::string& name)
: Character(position, name)
, m_MoveDirection()
{
    m_MoveDirection = mathgp::vc(0.f, 0.0f, 0.f);
}

void MonsterCharacter::SetMoveDirection(const mathgp::vector3& dir)
{
    m_MoveDirection = dir;
    if (m_MoveDirection != mathgp::vc(0.f, 0.f, 0.f))
    {
        m_MoveDirection.normalize();
    }
}

void MonsterCharacter::SetMoveSpeed(float speed)
{
    m_Speed = speed;
}

void MonsterCharacter::update(int dt)
{
    mathgp::vector3 offset = m_MoveDirection * (m_Speed * float(dt) * 0.001f);

    Move(m_pos + offset);

    Character::update(dt);
}
