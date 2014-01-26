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

#include "MonsterCharacter.h"

#include "MonsterDNA.h"
#include "Util.h"

#include "World.h"
#include "MainCharacter.h"

using namespace mathgp;

MonsterCharacter::MonsterCharacter(const mathgp::vector3& position, const std::string& name)
: Character(position, name)
, m_MoveDirection()
{
    m_MoveDirection = mathgp::vc(0.f, 0.0f, 0.f);
}

void MonsterCharacter::SetMoveDirection(const mathgp::vector3& dir)
{
    m_MoveDirection = dir;
    if (m_MoveDirection.length_sq() > 0.01f)
    {
        m_MoveDirection.normalize();
    }
}

void MonsterCharacter::SetTargetPoint(const point3& point)
{
    vector3 direction = point - position();
    SetMoveDirection(direction);
}

void MonsterCharacter::update(int dt)
{
    m_timeAtLastPosition += dt;

    think(dt);

    mathgp::vector3 offset = m_MoveDirection * (m_speed * float(dt) * 0.001f);

    Move(m_pos + offset);

    Character::update(dt);

    // check position change
    if (m_lastOwnPosition != position())
    {
        m_timeAtLastPosition = 0;
    }
}

inline float saturate(float f)
{
    return clamp(f, 0.01f, 1.f);
}

void MonsterCharacter::useDNA(const MonsterDNA& dna)
{
    m_dna = dna;

    // here be constants for max values
    static const float Max_HP = 1000;
    static const float Max_Stamina = 1000;
    static const float Max_Size = 5;
    static const float Max_Speed = 4;


    // determine stats
    m_size = 0.5f + Max_Size *
        dna(G_Size);

    // reduce speed by up to 40% based on size
    m_speed = Max_Speed *
        saturate(dna(G_Speed) - dna(G_Size) * 0.4f);

    // increase hp by up to 20% based on size
    m_hp = m_maxHp = int(Max_HP *
        saturate(dna(G_HP) + 0.2f * dna(G_Size)));

    // decrease stamina on small size
    m_stamina = m_maxStamina = int(Max_Stamina *
        saturate(dna(G_Stamina)));
    m_restCooldown = 0;
    m_neededRestTime = 500 + int(dna(G_Size) * 4000);

    m_aggroRange = g_worldSize * dna(G_AggroRange);
    m_aggroCooldown = int(dna(G_AggroCooldown) * 60000); // 1 minute
    m_currentAggro = 0;

    m_sightRange = g_worldSize * dna(G_Sight);
    m_hearingRange = g_worldSize * dna(G_Hearing);

    // 100 hp per 10 seconds
    // means 1 hp per 100 ms
    m_regenPer100ms = dna(G_Regeneration);

    m_damageDealtToPlayer = 0;
    m_lifetime = 0;

    m_hasLastKnownPlayerPosition = false;
    m_lastKnownPlayerPosition = Vec::zero;

    
    // idle 
    m_loiterCooldown = 0;
    m_hasPointToGoTo = false;
    m_pointToGoTo = Vec::zero;

    m_lastOwnPosition = position();
    m_timeAtLastPosition = 0;

    ////////////////////////////////////////////////
    // determine parts and weapons
    float maxWeapon = 0;
    int weapon = 0;
    for (int i = G_UseSpitter; i < G_UseThorns; ++i)
    {
        if (dna(i) > maxWeapon)
        {
            maxWeapon = dna(i);
            weapon = i;
        }
    }

    switch (weapon)
    {
    case G_UseSpitter:
        break;
    case G_UseGrapple:
        break;
    case G_UseClaws:
        break;
    case G_UseThorns:
        break;
    default:
        // no weapon!
        break;
    }

    float maxDefense = 0;
    int defense = G_UseSpitter;
    for (int i = G_NoDefense; i < G_UseScales; ++i)
    {
        if (dna(i) > maxDefense)
        {
            maxDefense = dna(i);
            defense = i;
        }
    }

    switch (defense)
    {
    case G_UseScales:
        break;
    case G_UseFatness:
        break;
    case G_NoDefense:
    default:
        break;
    }
}

MonsterDNA MonsterCharacter::giveOffspring()
{
    MonsterDNA newdna = m_dna;
    newdna.mutate();
    return newdna;
}

void MonsterCharacter::heal(int hp)
{
    m_hp += hp;

    if (m_hp > m_maxHp)
        m_hp = m_maxHp;
}

void MonsterCharacter::think(int dt)
{
    if (isDead())
    {
        SetMoveDirection(Vec::zero);
        return; // duh
    }

    ////////////////////////////////////////////
    // do cooldowns
    
    // regen
    m_regenPer100ms += float(dt) / 100;
    while (m_regenPer100ms >= 1)
    {
        heal(1);
        --m_regenPer100ms;
    }

    // aggro
    if (hasAggro())
    {
        m_aggroCooldown -= dt;
        if (m_aggroCooldown < 0) m_aggroCooldown = 0;
    }

    // stamina
    if (isTired())
    {
        m_restCooldown -= dt;
        if (m_restCooldown < 0) m_restCooldown = 0;
    }

    if (isLoitering())
    {
        m_loiterCooldown -= dt;
        if (m_loiterCooldown < 0) m_loiterCooldown = 0;
    }

    ////////////////////////////////////////////
    // senses


    ////////////////////////////////////////////
    // behave
    if (!hasAggro())
    {
        // no aggro -> stroll aimlessly around the world
        // or loiter
        if (isLoitering())
        {
            return;
        }

        if (m_hasPointToGoTo)
        {
            if (distance(position(), m_pointToGoTo) > 0.3f)
            {
                // if we're here a long time, we're probably stuck
                if (m_timeAtLastPosition < 1000)
                {
                    SetTargetPoint(m_pointToGoTo);
                    return;
                }
            }
            // else we have arrived
        }

        if (Util::Rnd11() < 0)
        {
            // loiter up to 3 seconds
            m_loiterCooldown = int(Util::Rnd01() * 3000);
        }
        else
        {
            // choose point to go to in sight range
            auto point = vc(Util::Rnd11(), Util::Rnd11(), Util::Rnd11());

            point *= m_sightRange;
            point += position();

            SetTargetPoint(point);
        }
    }



    World& world = World::instance();

    MainCharacter* enemy = world.mainCharacter();

    //if (no)
}
