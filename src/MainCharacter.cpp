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
#include "MainCharacter.h"
#include "GameplayConstants.h"
#include "ColliderGrid.h"
#include "Application.h"
#include "GameState.h"
#include "Camera.h"
#include "World.h"
#include "SoundManager.h"
#include "DeathState.h"

MainCharacter::MainCharacter(const mathgp::vector3& position, const std::vector<AttackData>& attacks)
: Character(position, "player", attacks)
{
    m_weapon = new PlayerWeapon(EPistol, 1000, mathgp::v((unsigned)1,(unsigned)10));
    m_hp = Player_Starting_Health;
}

void MainCharacter::update(int dt)
{
    if (isDead() && m_PlayerAnimationController.isReadyToDiscard())
    {
        World::instance().destroyObject(m_id);
        return;
    }
    m_PlayerAnimationController.update(m_pos, Application::instance().currentState()->camera()->direction());
}

void MainCharacter::OnHit(EAttackDamageType dmgType, int dmg)
{
    if (dmgType != EMonsterDamage)
        return; //not concerned by own dmg

    rawDamage(dmg);
}

void MainCharacter::useWeapon(const mathgp::vector3& worldPoint, Object* objectHit)
{
    m_weapon->attack(worldPoint, objectHit);
}

void MainCharacter::Die()
{
    SoundManager::instance().playSound(ESounds_PlayerDeath);
    //should something else happen?
    GameState* state = new DeathState;
    Application::instance().pushState(state);
}

void MainCharacter::GetDamage()
{
    m_PlayerAnimationController.GetDamage();
}

void MainCharacter::Attack(Uint32 attackIndex)
{
    m_PlayerAnimationController.Attack();
}

void MainCharacter::Move(const mathgp::vector3& position)
{
    const mathgp::vector3 oldPosition = m_pos;

    std::shared_ptr<Object> collision = ColliderGrid::instance().requestMoveTo(this, position);

    if (collision)
    {
        return;
    }

    if (m_pos.x() >= oldPosition.x())
    {
        m_PlayerAnimationController.SetMovement(PM_Right);
    }
    else
    {
        m_PlayerAnimationController.SetMovement(PM_LEft);
    }
}

void MainCharacter::GoIdle()
{
    m_PlayerAnimationController.SetMovement(PM_Idle);
}
