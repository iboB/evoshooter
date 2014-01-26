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


enum EWeaponType
{
    EKnife = 0,
    EPistol,
    EWeaponCount
};

enum EAttackType
{
    EMele,
    EShoot
};

class PlayerWeapon
{
public:
    PlayerWeapon(EWeaponType t, unsigned attackDelay);
    ~PlayerWeapon();

    void attack(const mathgp::vector3& worldPoint, Object* objectHitWithCursor);
    static std::string m_weaponNames[EWeaponCount]; //what to show in the gui
    static EAttackDamageType m_weaponDamageTypes[EWeaponCount];
    static EAttackType m_weaponAttackTypes[EWeaponCount];
private:
    EWeaponType m_type;
    unsigned int m_attackDelay;
    unsigned int m_lastAttackTimestamp;
    EAttackType m_attackType;
    EAttackDamageType m_damageType;

    

    void meleeAttack(const mathgp::vector3& worldPoint);
};