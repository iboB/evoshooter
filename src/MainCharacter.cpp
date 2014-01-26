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

MainCharacter::MainCharacter(const mathgp::vector3& position, const std::vector<AttackData>& attacks)
: Character(position, "eye", attacks)
{
    m_weapon = new PlayerWeapon(EPistol, 1000, mathgp::v((unsigned)1,(unsigned)10));
    m_hp = Player_Starting_Health;
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
    m_weapon->attack(worldPoint, objectHit);
}
