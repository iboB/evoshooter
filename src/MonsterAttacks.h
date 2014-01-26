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

class MonsterCharacter;
class MonsterDNA;

class MonsterAttack
{
public:
    MonsterAttack() : m_owner(nullptr) {}

    void setOwner(MonsterCharacter* owner);

    virtual float range() const = 0;
    virtual float senseOfRange() const = 0;
    virtual void attack(const mathgp::point3& point) = 0;
    virtual void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point) = 0;

protected:
    MonsterCharacter* m_owner;
};

class RangedAttack : public MonsterAttack
{
public:
    float range() const override { return Close_Distance_In_World; }
    float senseOfRange() const override { return Close_Distance_In_World; }

    mathgp::point3 aimAt(const mathgp::point3& point)
    {

    }

    void attack(const mathgp::point3& point)
    {
        
    }

    void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point)
    {
        attack(point);
    }
};

class MeleeAttack : public MonsterAttack
{
public:
    float range() const override { return m_range; }
    float senseOfRange() const override
    {
        return m_range / m_owner->dna()(G_SenseOfOwnRange);
    }

    void attack(const mathgp::point3& point)
    {

    }

    void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point)
    {
        attack(point);
    }


    float m_range;
};

class NoAttack : public MonsterAttack
{
    float range() const override { return 0; }
    float senseOfRange() const override { return 0; }

    mathgp::point3 aimAt(const mathgp::point3& point)
    {}

    void attack(const mathgp::point3& point)
    {}

    void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point)
    {}

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

