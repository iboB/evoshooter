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

#include "MonsterCharacter.h"
#include "GameplayConstants.h"
#include "World.h"
#include "MainCharacter.h"
#include "RegisteredMonsterDamage.h"

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
        int now = SDL_GetTicks();
        if (now - m_lastAttackTime < m_cooldownTime)
        {
            // cant attack
            return;
        }

        m_owner->Attack(0);

        using namespace mathgp;
        
        point3 at = aimAt(lastKnownPosition, point);

        float dist = distance(at, m_owner->position());

        if (dist > range())
        {
            vector3 atDirection = at - m_owner->position();
            at = m_owner->position() + range() * normalized(atDirection);
        }

        // btodo: distort at based og G_PRECISION

        RegisteredMonsterDamage dealDmg;

        dealDmg.damage = m_damage;
        dealDmg.damageType = m_damageType;
        dealDmg.ownerId = m_owner->id();
        dealDmg.position = at;
        dealDmg.raidus = m_aoeRange;
        
        dealDmg.remainingTime = int(1000 * (dist / m_projectileSpeed)); // ms to s conversion

        World::instance().registerMonsterDamage(dealDmg);

        m_lastAttackTime = now;
    }

    virtual mathgp::point3 aimAt(const mathgp::point3& prevPoint, const mathgp::point3& curPoint) = 0;    

//protected:
    MonsterCharacter* m_owner;

    float m_staminaCostPerHit;
    int m_cooldownTime;
    int m_lastAttackTime;
    float m_range;
    float m_aoeRange;
    float m_projectileSpeed;
    int m_damage;
    int m_damageType;
};

class RangedAttack : public MonsterAttack
{
public:
    mathgp::point3 aimAt(const mathgp::point3& prevPoint, const mathgp::point3& curPoint) override
    {
        // btodo : G_AttackDesire
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

