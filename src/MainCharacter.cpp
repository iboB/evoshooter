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
