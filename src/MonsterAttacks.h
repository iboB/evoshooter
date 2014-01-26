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

class MonsterCharacter;
class MonsterDNA;

class MonsterAttack
{
public:
    MonsterAttack() : m_owner(nullptr) {}

    void setOwner(MonsterCharacter* owner);

    virtual float range() const = 0;
    virtual float senseOfRange() = 0;
    virtual void attack(const mathgp::point3& point) = 0;
    virtual void attack(const mathgp::point3& lastKnownPosition, const mathgp::point3& point) = 0;

private:
    MonsterCharacter* m_owner;
};

class RangedAttack : public MonsterAttack
{

};

class MeleeAttack : public MonsterAttack
{

};

class NoAttack : public MonsterAttack
{

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

