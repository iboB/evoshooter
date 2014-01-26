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
#include "EvoShooter.pch.h"
#include "MonsterDNA.h"
#include "Util.h"

using namespace std;
using namespace mathgp;

float sgn(float f)
{
    return f < 0 ? -1.f : 1.f;
}

void Gene::randomize()
{
    m_value = Util::Rnd01();
}

void Gene::mutate()
{
    float mutationPercent = Util::Rnd01();
    float mutationSign = sgn(Util::Rnd11());

    float factor = 0;
    
    // silly lower bound
    for (auto lvl : m_mutationLevel)
    {
        if (lvl.x() >= mutationPercent)
        {
            break;
        }

        factor = lvl.y();
    }

    m_value += mutationSign * factor;
    
    //silly clamp
    if (m_value < 0)
        m_value = 0;
    else if (m_value > 1)
        m_value = 1;
}

MonsterDNA::MonsterDNA()
    : m_cost(0)
    , m(Num_Genes)
{
    // just pump all the mutation levels
    vector<vector2> commonMutationLevel;
    commonMutationLevel.push_back(vc(0, 0));
    commonMutationLevel.push_back(vc(0.50f, 0.05f));
    commonMutationLevel.push_back(vc(0.95f, 0.1f));

    for (auto& g : m)
    {
        g.setMutationLevel(commonMutationLevel);
        g.setCostFactor(1);
    }

    // no clear cost here
    m[G_Size].setCostFactor(0);
    m[G_Aim].setCostFactor(0);
    m[G_HardHit].setCostFactor(0);
    m[G_UseClaws].setCostFactor(0);
    m[G_UseSpitter].setCostFactor(0);
    m[G_UseGrapple].setCostFactor(0);
    m[G_UseThorns].setCostFactor(0);
}

float MonsterDNA::calculateCost() const
{
    if (m_cost != 0)
        return m_cost;

    m_cost = 0;
    for (const auto& g : m)
    {
        m_cost += g.cost();
    }

    return m_cost;
}

void MonsterDNA::mutate()
{
    m_cost = 0;

    for (auto& g : m)
    {
        g.mutate();
    }

    calculateCost();
}

void MonsterDNA::randomize()
{
    for (auto& g : m)
    {
        g.randomize();
    }
}

float MonsterDNA::operator()(int g) const
{
    assert(g < Num_Genes);
    return m[g].value();
}


