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

enum GeneType
{
    G_DesiredAttackRange_RangedEnemy, // 1 next to the player - 0 max far away
    G_DesiredAttackRange_MeleeEnemy, // 
    G_MovementPrecision_RangedEnemy, // 1 aims for attack range - 0 total randomness
    G_MovementPrecision_MeleeEnemy,
    G_AttackDesire, // 1 only when player is in range - 0 total randomness
    G_Aim, // 1 shoots for player, 0.5 interpolates player position, 1 extrapolates player position
    G_Precision, // 1 exactly as needed, 0 - wide range
    G_HP, // percent of max - guideline
    G_Speed, // percent of max - guideline
    G_Size, // percent of max - NO guideline
    G_Stamina, // percent of max - guideline
    // PanicThreshold, // what percentage of HP will panic and go total random

    /// Attacks
    G_UseSpitter,
    G_UseGrapple,
    G_UseClaws,
    G_UseThorns,

    G_SpitterPower,
    G_GrapplePower,
    G_ClawsPower,
    G_ThornsPower,

    /// Defenses
    G_NoDefense,
    G_Fatness,
    G_Scales,
    
    G_FatnessPower,
    G_ScalesPower,
    
    ///
    Num_Genes
};

class Gene
{
public:
    Gene()
        : m_costFactor(1)
        , m_value(0)
    {}

    void randomize();
    void mutate();

    float value() const { return m_value; }

    void setCostFactor(float costFactor) { m_costFactor = costFactor; }
    float cost() const { return m_costFactor * m_value; }

    void setMutationLevel(const std::vector<mathgp::vector2>& mutationLevel) { m_mutationLevel = mutationLevel; }

private:
    float m_value;
    float m_costFactor;

    // pairs sorted by x
    // first is value between 0 and 1, second is mutation factor
    // ex 
    // (0, 0), (0.5, 0.05), (0.9, 0.1)
    // 50% no mutation
    // 45% mutation by 0.05
    // 5% mutation by 0.1
    // ideally this should be shared by all genes but... no time
    std::vector<mathgp::vector2> m_mutationLevel;
};



class MonsterDNA
{
public:
    MonsterDNA();

    float calculateCost() const; // cost to reproduce.
    void mutate();
    void randomize();

//private:

    float operator()(GeneType g) const;

    std::vector<Gene> m;

    mutable float m_cost;
};