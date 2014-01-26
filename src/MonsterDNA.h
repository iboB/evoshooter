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

enum GeneType
{
    G_Aggresiveness, // 0 - none, 1 - 100%
    G_AggroCooldown, // aggro cooldown - 0 immediately - 1: minutes
    G_Sight, // 0 blind - 1 sees entire map
    G_Hearing, // 0 deaf - 1 hears entire map
    G_Empathy, // 0 doesn't care if not hit - 1 aggro on every attack in sight/hearing
    G_SenseOfOwnRange, // 0 random attacks - 1 attack only in range
    G_AttackDesire, // 1 only when player is in range - 0 total randomness (ie 1 - chance to perform random attack)
    //G_Aim, // no clear cost???? extrapolate a part of a unit in front of a player
    //G_Precision, // 1 exactly as needed, 0 - wide range (meaningless for mellee)
    G_HardHit, // bonus melee dmg for longer cooldown and more stamina cost
    G_HP, // percent of max - guideline
    G_Speed, // percent of max - guideline
    G_Size, // percent of max - NO guideline
    G_Stamina, // percent of max - guideline
    G_Regeneration, // how much time to regen 100 hp
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
    G_UseFatness,
    G_UseScales,
    
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

    float operator()(int g) const;

    std::vector<Gene> m;

    mutable float m_cost;
};