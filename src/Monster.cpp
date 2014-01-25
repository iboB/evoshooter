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
#include "EvoShooter.pch.h"
#include "Monster.h"
#include "MonsterDNA.h"
#include "Util.h"

using namespace mathgp;

inline float saturate(float f)
{
    return clamp(f, 0.01f, 1.f);
}

void Monster::useDNA(const MonsterDNA& dna)
{
    m_dna = dna;

    // here be constants for max values
    static const float Max_HP = 1000;
    static const float Max_Stamina = 1000;
    static const float Max_Size = 5;
    static const float Max_Speed = 5;

    // determine stats
    m_size = Max_Size *
        dna(G_Size);
    
    // reduce speed by up to 40% based on size
    m_speed = Max_Speed *
        saturate(dna(G_Speed) - dna(G_Size) * 0.4f);

    // increase hp by up to 20% based on size
    m_hp = Max_HP *
        (dna(G_HP) + 0.2f * dna(G_Size));

    // decrease stamina on small size
    //m_stamina = Max_Stamina *
    //    (gna(G_Stamina) - )
    
}

MonsterDNA Monster::giveOffspring()
{
    MonsterDNA newdna = m_dna;
    newdna.mutate();
    return newdna;
}