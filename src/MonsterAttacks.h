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

#include "MonsterCharacter.h"
#include "GameplayConstants.h"
#include "World.h"
#include "MainCharacter.h"

class MonsterCharacter;
class MonsterDNA;

class MonsterAttack
{
public:
    MonsterAttack() : m_owner(nullptr), m_lastAttackTime(0) {}

    void setOwner(MonsterCharacter* owner);

    float range() const { return m_range; }
    
    float senseOfRange() const
    {
        return m_range / m_owner->dna()(G_SenseOfOwnRange);
    }

    virtual void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point)
    {

    }

    virtual mathgp::point3 aimAt(const mathgp::point3& prevPoint, const mathgp::point3& curPoint) = 0;    

protected:
    MonsterCharacter* m_owner;

    float m_staminaCostPerHit;
    int m_cooldownTime;
    int m_lastAttackTime;
    float m_range;
    float m_aoeRange;
    float m_projectileSpeed;
};

class RangedAttack : public MonsterAttack
{
public:
    mathgp::point3 aimAt(const mathgp::point3& prevPoint, const mathgp::point3& curPoint) override
    {
        return curPoint;
    }
};

class MeleeAttack : public MonsterAttack
{
public:
    mathgp::point3 aimAt(const mathgp::point3& prevPoint, const mathgp::point3& curPoint) override
    {
        return curPoint;
    }
};

class NoAttack : public MonsterAttack
{
    mathgp::point3 aimAt(const mathgp::point3& prevPoint, const mathgp::point3& curPoint)
    {
        return Vec::zero;
    }

    void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point) override {}
};

//class Thorns : public RangedAttack
//{
//
//};
//
//class Spit : public RangedAttack
//{
//
//};
//
//
//class Grapple : public MeleeAttack
//{
//
//};
//
//class Claws : public MeleeAttack
//{
//
//};

