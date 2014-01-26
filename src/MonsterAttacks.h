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

    //virtual float senseOfRange() const;
    //virtual void attack(const mathgp::point3& point);

private:
    MonsterCharacter* m_owner;
};

class RangedAttack : public MonsterAttack
{

};

class Thorns : public RangedAttack
{

};

class Spit : public RangedAttack
{

};

class MeleeAttack : public MonsterAttack
{

};

class Grapple : public MeleeAttack
{

};

class Claws : public MeleeAttack
{

};

class NoAttack : public MonsterAttack
{

};