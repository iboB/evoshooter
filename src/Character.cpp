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

#include "Character.h"
#include "ColliderGrid.h"
#include "Application.h"
#include "GameState.h"
#include "Camera.h"

Character::Character(const mathgp::vector3& position, const std::string& name)
: Object(position, 0.3f)
{
    m_AnimationsController.SetMovementAnimations(std::string("sprites/")  + name + "_walk_anim.png", 0.008f);
    m_AnimationsController.SetDamage(std::string("sprites/") + name + "_dmg_anim.png", 0.008f);
    m_AnimationsController.SetDeath(std::string("sprites/") + name + "_die_anim.png", 0.008f);
    m_AnimationsController.AddAttack("sprites/attacks/attack_anim_01.png", "sprites/attacks/attack_anim_idle_01.png", Vec::zero, 0.003f);
    m_AnimationsController.AddAttack("sprites/attacks/attack_anim_02.png", "sprites/attacks/attack_anim_idle_02.png", Vec::zero, 0.003f);
    m_AnimationsController.AddAttack("sprites/attacks/attack_anim_03.png", "sprites/attacks/attack_anim_idle_03.png", Vec::zero, 0.003f);
}

void Character::Move(const mathgp::vector3& position)
{
    const mathgp::vector3 oldPosition = m_pos;

    std::shared_ptr<Object> collision = ColliderGrid::instance().requestMoveTo(this, position);

    if (collision)
    {
        return;
    }

    if (m_pos.x() >= oldPosition.x())
    {
        m_AnimationsController.SetMove(MA_Right);
    }
    else
    {
        m_AnimationsController.SetMove(MA_Left);
    }
}

void Character::Die()
{
    m_AnimationsController.Die();
}

void Character::GetDamage()
{
    m_AnimationsController.GetDamage();
}

void Character::Attack(Uint32 attackIndex)
{
    m_AnimationsController.Attack(attackIndex);
}

void Character::update(int dt)
{
    m_AnimationsController.update(m_pos, Application::instance().currentState()->camera()->direction());
}
