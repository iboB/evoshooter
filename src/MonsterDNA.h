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

class Gene
{
public:
    virtual void mutate() = 0;
};

class MonsterDNA
{
    // [2] means per player attack type (close, ranged)

    float desiredAttackRange[2]; // 0 next to the player - 1 max far away
    float movementRandomness[2]; // 0 aims for attack range - 1 total randomness
    float attackRandomness; // 0 only when player is in range - 1 total randomness

    float aim; // shoots for player, 0.5 interpolates player position, 1 extrapolates player position

    float hp;
    float speed;
    float size; // size factors on hp and speed
    
    float panicThreshold; // what percentage of HP will panic and go total random

    // attacks (max attack wins)
    float spitter;    
    float otherAttack;

    float spitterPower;
    float otherAttackPower; // power of different attacks

    // float defenses (max defense wins)
    float scales;
    float fat;

    float scalesPower; // power of different defenses

    int cost(); // cost to reproduce. 
};