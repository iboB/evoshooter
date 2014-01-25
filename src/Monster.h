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

class Monster
{
public:

    void randomize();
    
    float calculateFitness() const; // damage * lifetime

private:
    MonsterDNA m_dna;

    float m_damageDealtToPlayer;
    float m_lifetime;
};