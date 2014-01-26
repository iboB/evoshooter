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

MainCharacter::MainCharacter(const mathgp::vector3& position, const std::vector<AttackData>& attacks)
: Character(position, "eye", attacks)
{
    m_currentWeapon = new PlayerWeapon(EPistol, 1000, mathgp::v((unsigned)Pistol_Damage_Min, (unsigned)Pistol_Damage_Max)); //pistol
    m_weapons.push_back(m_currentWeapon);
    m_currentWeapon = new PlayerWeapon(EKnife, 1000, mathgp::v((unsigned)Knife_Damage_Min, (unsigned)Knife_Damage_Max));
    m_weapons.push_back(m_currentWeapon);

    m_currentWeaponIndex = 1;

    m_hp = Player_Starting_Health;
    m_maxHp = Player_Starting_Health;

    ((ExperimentState*)(Application::instance().baseState()))->weaponDisplay(m_currentWeapon->m_weaponNames[m_currentWeapon->type()]);

}

MainCharacter::~MainCharacter()
{
    for (int i = 0; i < m_weapons.size(); ++i)
    {
        delete m_weapons[i];
    }
    m_weapons.clear();
}

void MainCharacter::nextWeapon()
{
    ++m_currentWeaponIndex;
    if (m_currentWeaponIndex >= m_weapons.size())
    {
        m_currentWeaponIndex = 0;
    }
    m_currentWeapon = m_weapons[m_currentWeaponIndex];

    ((ExperimentState*)(Application::instance().baseState()))->weaponDisplay(m_currentWeapon->m_weaponNames[m_currentWeapon->type()]);
}

void MainCharacter::previousWeapon()
{
    --m_currentWeaponIndex;
    if (m_currentWeaponIndex < 0)
    {
        m_currentWeaponIndex = m_weapons.size() - 1;
    }
    m_currentWeapon = m_weapons[m_currentWeaponIndex];

    ((ExperimentState*)(Application::instance().baseState()))->weaponDisplay(m_currentWeapon->m_weaponNames[m_currentWeapon->type()]);
}

void MainCharacter::update(int dt)
{
    Character::update(dt);
}

void MainCharacter::OnHit(EAttackDamageType dmgType, int dmg)
{
    if (dmgType != EMonsterDamage)
        return; //not concerned by own dmg

    rawDamage(dmg);
}

void MainCharacter::useWeapon(const mathgp::vector3& worldPoint, Object* objectHit)
{
    m_currentWeapon->attack(worldPoint, objectHit);
}
