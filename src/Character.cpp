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

#include "Character.h"
#include "ColliderGrid.h"
#include "Application.h"
#include "GameState.h"
#include "Camera.h"
#include "World.h"
#include "SoundManager.h"

#include "DeathState.h"

Character::Character(const mathgp::vector3& position, const std::string& name, const std::vector<AttackData>& attacks)
: Object(position, 1.0f)
{
    init(name, attacks);
}

Character::Character(const mathgp::vector3& position)
: Object(position, 1.0f)
{

}

void Character::init(const std::string& name, const std::vector<AttackData>& attacks)
{
    m_renderShadow = true;
 
    if (name == "player")
        return;

    m_AnimationsController.SetMovementAnimations(std::string("sprites/")  + name + "_walk_anim.png", 0.008f);
    m_AnimationsController.SetDamage(std::string("sprites/") + name + "_dmg_anim.png", 0.008f);
    m_AnimationsController.SetDeath(std::string("sprites/") + name + "_die_anim.png", 0.008f);

    for (auto it = attacks.begin(); it != attacks.end(); ++it)
    {
        m_AnimationsController.AddAttack(it->AttackName, it->IdleName, it->Offset, it->Scale, it->IsWholeBodyAttack);
    }

    m_AnimationsController.SetMove(MA_Left);
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
    if (m_type == EMonster_Character)
    {
        SoundManager::instance().playSound(ESounds_EnemyDeath);
    }
    else if (m_type == EPlayer_Character)
    {
        SoundManager::instance().playSound(ESounds_PlayerDeath);
        //should something else happen?
        GameState* state = new DeathState;
        Application::instance().pushState(state);
    }
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
    if (isDead() && m_AnimationsController.isReadyToDiscard())
    {
        World::instance().destroyObject(m_id);
        return;
    }
    m_AnimationsController.update(m_pos, Application::instance().currentState()->camera()->direction());
}

void Character::heal(int hp)
{
    m_hp += hp;

    if (m_hp > m_maxHp)
        m_hp = m_maxHp;
}
void Character::OnHit(EAttackDamageType dmgType, int dmg)
{

}
int Character::rawDamage(int dmg)
{
    m_hp -= dmg;
   // SoundManager::instance().playSound();
    if (m_hp <= 0)
    {
        //dead;
        Die();        
    }
    else
    {
        GetDamage();
    }

    return dmg;
}
