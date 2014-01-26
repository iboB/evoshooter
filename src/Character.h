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

#include "Object.h"

#include "AnimationsController.h"

struct AttackData
{
    std::string AttackName;
    std::string IdleName;
    mathgp::vector3 Offset;
    float Scale;
    bool IsWholeBodyAttack;
};

typedef std::vector<AttackData> AttacksData;

class Character : public Object
{
public:
    Character(const mathgp::vector3& position, const std::string& name, const std::vector<AttackData>& attacks);

    virtual void Move(const mathgp::vector3& position);
    virtual void Die();
    virtual void GetDamage();
    virtual void Attack(Uint32 attackIndex);
    virtual void OnHit(EAttackDamageType dmgType, int dmg);

    virtual void update(int dt);
    void heal(int hp);
    void rawDamage(int dmg);
protected:
    AnimationsController m_AnimationsController;
    int m_maxHp;
    int m_hp;    
};
