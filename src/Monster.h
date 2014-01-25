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

#include "MonsterDNA.h"

class MonsterDNA;

class Monster
{
public:
    void useDNA(const MonsterDNA& dna);
    MonsterDNA giveOffspring();

    float calculateFitness() const; // damage * lifetime

private:
    MonsterDNA m_dna;

    // stats
    float m_hp;
    float m_stamina;
    float m_speed;
    float m_size;

    float m_damageDealtToPlayer;
    float m_lifetime;
};