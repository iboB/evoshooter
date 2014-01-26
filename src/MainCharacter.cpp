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
#include "Application.h"
#include "ExperimentState.h"
#include "SoundManager.h"
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
    m_currentWeapon = new PlayerWeapon(EPistol, 1000, mathgp::v((unsigned)Pistol_Damage_Min, (unsigned)Pistol_Damage_Max)); //pistol
    m_weapons.push_back(m_currentWeapon);
    m_currentWeapon = new PlayerWeapon(EKnife, 1000, mathgp::v((unsigned)Knife_Damage_Min, (unsigned)Knife_Damage_Max));
    m_weapons.push_back(m_currentWeapon);
    m_currentWeapon = new PlayerWeapon(EShotgun, 1000, mathgp::v((unsigned)Shotgun_Damage_Min, (unsigned)Shotgun_Damage_Max));
    m_weapons.push_back(m_currentWeapon);

    m_currentWeaponIndex = 1;

    m_hp = Player_Starting_Health;
    m_maxHp = Player_Starting_Health;

    ((ExperimentState*)(Application::instance().baseState()))->weaponDisplay(m_currentWeapon->m_weaponNames[m_currentWeapon->type()]);

}

MainCharacter::~MainCharacter()
{
    for (unsigned i = 0; i < m_weapons.size(); ++i)
    {
        delete m_weapons[i];
    }
    m_weapons.clear();
}

void MainCharacter::nextWeapon()
{
    ++m_currentWeaponIndex;
    if (m_currentWeaponIndex >= (int)m_weapons.size())
    {
        m_currentWeaponIndex = 0;
    }
    m_currentWeapon = m_weapons[m_currentWeaponIndex];

    switch (m_currentWeaponIndex)
    {
    case 0:
        m_PlayerAnimationController.SetWeapon(PWT_Pistol);
        break;
    case 1:
        m_PlayerAnimationController.SetWeapon(PWT_Sword);
        break;
    case 2:
        m_PlayerAnimationController.SetWeapon(PWT_Shotgun);
        break;
    default:
        break;
    }

    ((ExperimentState*)(Application::instance().baseState()))->weaponDisplay(m_currentWeapon->m_weaponNames[m_currentWeapon->type()]);
}

void MainCharacter::previousWeapon()
{
    --m_currentWeaponIndex;
    if (m_currentWeaponIndex < 0)
    {
        m_currentWeaponIndex = m_weapons.size() - 1;
    }

    switch (m_currentWeaponIndex)
    {
    case 0:
        m_PlayerAnimationController.SetWeapon(PWT_Pistol);
        break;
    case 1:
        m_PlayerAnimationController.SetWeapon(PWT_Sword);
        break;
    case 2:
        m_PlayerAnimationController.SetWeapon(PWT_Shotgun);
        break;
    default:
        break;
    }

    m_currentWeapon = m_weapons[m_currentWeaponIndex];

    ((ExperimentState*)(Application::instance().baseState()))->weaponDisplay(m_currentWeapon->m_weaponNames[m_currentWeapon->type()]);
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

    if (m_PlayerAnimationController.isDead())
    {
        return;
    }

    rawDamage(dmg);
}

void MainCharacter::useWeapon(const mathgp::vector3& worldPoint, Object* objectHit)
{
    SoundManager::instance().playSound(ESounds_Shoot);
    m_currentWeapon->attack(worldPoint, objectHit);

    m_PlayerAnimationController.Attack();
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
