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
    PlayerWeapon(EWeaponType t, unsigned attackDelay, const mathgp::uvector2& damageRange);
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

    mathgp::uvector2 m_damageRange;

    
    unsigned damage();
    void meleeAttack(const mathgp::vector3& worldPoint);
    void rangedAttack(const mathgp::vector3& targetPos);
};