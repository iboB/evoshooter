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
#include "PlayerWeapon.h"

class MainCharacter : public Character
{
public:
    MainCharacter(const mathgp::vector3& position, const std::vector<AttackData>& attacks);
    ~MainCharacter();

    void OnHit(EAttackDamageType dmgType, int dmg);
    virtual void update(int dt);
    void useWeapon(const mathgp::vector3& worldPoint, Object* objectHit);

    void nextWeapon();
    void previousWeapon();
private:
    PlayerWeapon* m_currentWeapon;
    std::vector<PlayerWeapon*> m_weapons;
    int m_currentWeaponIndex;
};