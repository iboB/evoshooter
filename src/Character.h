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
    Character(const mathgp::vector3& position);

    void init(const std::string& name, const std::vector<AttackData>& attacks);

    virtual void Move(const mathgp::vector3& position);
    virtual void Die();
    virtual void GetDamage();
    virtual void Attack(Uint32 attackIndex);
    virtual void OnHit(EAttackDamageType dmgType, int dmg);

    virtual void update(int dt);
    void heal(int hp);
    int rawDamage(int dmg);
    int isDead() { return m_hp <= 0; }

    int maxHp() const { return m_maxHp; }
    int hp() const { return m_hp; }
protected:
    AnimationsController m_AnimationsController;
    int m_maxHp;
    int m_hp;    
};
