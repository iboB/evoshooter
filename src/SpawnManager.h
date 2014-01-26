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

#include "ImplicitSingleton.h"

class MonsterCharacter;

class SpawnManager : public ImplicitSingleton<SpawnManager>
{
public:
    SpawnManager();

    void update(int dt);

    void onMonsterDeath(MonsterCharacter* monster);

private:
    int m_time;
    
    float m_costRise;
    
    float m_targetWorldCost;
    float m_actualWorldCost;
};