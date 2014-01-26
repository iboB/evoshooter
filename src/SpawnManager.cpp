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
#include "SpawnManager.h"

#include "GameplayConstants.h"

#include "World.h"

using namespace std;
using namespace mathgp;

SpawnManager::SpawnManager()
: m_time(0)
, m_targetWorldCost(Initial_Target_World_Cost)
, m_actualWorldCost(0)
{

}

void SpawnManager::update(int dt)
{
    m_time += dt;
    
    m_costRise += float(dt)/1000;
    while (m_costRise > 1)
    {
        m_targetWorldCost += Target_World_Cost_Rise_Per_Second;
    }

    World& w = World::instance();

    if (m_targetWorldCost > m_actualWorldCost)
    {
        vector<const MonsterCharacter*> top4;

        // find top 4 monsters
        // spawn random of those
        for (const auto& pair : w.objects())
        {
            auto obj = pair.second.get();

            if (obj->type() != EMonster_Character)
            {
                continue;
            }

            auto monster = static_cast<const MonsterCharacter*>(obj);

            float curFitness = monster->fitness();

            if (top4.size() < 4)
            {
                top4.push_back(monster);
                sort(top4.begin(), top4.end(), [](const MonsterCharacter* a, const MonsterCharacter* b) -> bool
                {
                    return a->fitness() < b->fitness();
                });
            }
            else if (top4.back()->fitness() < curFitness)
            {
                top4.back() = monster;
                sort(top4.begin(), top4.end(), [](const MonsterCharacter* a, const MonsterCharacter* b) -> bool
                {
                    return a->fitness() < b->fitness();
                });
            }
        }

        if (!top4.empty())
        {
            while (m_targetWorldCost > m_actualWorldCost)
            {
                auto monster = top4[rand() % top4.size()];

                MonsterDNA dna = monster->dna();
                dna.mutate();

                w.spawnMonster(dna);

                m_actualWorldCost += dna.calculateCost();
            }
        }
        else
        {
            while (m_targetWorldCost > m_actualWorldCost)
            {
                MonsterDNA dna;
                dna.randomize();

                w.spawnMonster(dna);

                m_actualWorldCost += dna.calculateCost();
            }
        }
    }
}

void SpawnManager::onMonsterDeath(MonsterCharacter* monster)
{
    MonsterDNA dna = monster->dna();

    m_actualWorldCost -= dna.calculateCost();
}