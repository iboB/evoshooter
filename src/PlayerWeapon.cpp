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
#include "PlayerWeapon.h"
#include "World.h"
#include "ColliderGrid.h"
#include "MainCharacter.h"

std::string PlayerWeapon::m_weaponNames[EWeaponCount] =
{
    "Knife", "Gun"
};
EAttackDamageType PlayerWeapon::m_weaponDamageTypes[EWeaponCount] =
{
    EPiercing, EBlunt
};
EAttackType PlayerWeapon::m_weaponAttackTypes[EWeaponCount] =
{
    EMele, EShoot
};

using namespace mathgp;

PlayerWeapon::PlayerWeapon(EWeaponType t, unsigned attackDelay):
m_type(t)
{
    m_attackType = m_weaponAttackTypes[m_type];
    m_damageType = m_weaponDamageTypes[m_type];
    m_attackDelay = attackDelay;
    m_lastAttackTimestamp = 0;
}
PlayerWeapon::~PlayerWeapon()
{
}

void PlayerWeapon::attack(const mathgp::vector3& worldPoint, Object* objectHit)
{
    unsigned int now = SDL_GetTicks();
    if (now >= m_lastAttackTimestamp + m_attackDelay)
    {
        now = SDL_GetTicks();
        switch (m_type)
        {
        case EKnife:
            meleeAttack(worldPoint);
            break;
        case EPistol:
            break;
        }
    }
}

void PlayerWeapon::meleeAttack(const mathgp::vector3& worldPoint)
{
    vector3 playerPos = World::instance().mainCharacter()->position();
    vector3 directionOfAttack = normalized(worldPoint - playerPos);
    vector3 pointOfAttack = directionOfAttack*0.4 + playerPos;


    std::vector< std::shared_ptr<Object> > affectedTargets = ColliderGrid::instance().collideWithCircle(pointOfAttack.xy(), 0.5f);
    std::vector< std::shared_ptr<Object> >::iterator it = affectedTargets.begin();
    while (it != affectedTargets.end())
    {
        (*it)->OnHit(m_damageType, 5);
        ++it;
    }
}