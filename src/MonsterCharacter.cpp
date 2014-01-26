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
#include "ColliderGrid.h"

#include "MonsterAttacks.h"

#include "GameplayConstants.h"

using namespace mathgp;

MonsterCharacter::MonsterCharacter(const mathgp::vector3& position, const std::string& name, const std::vector<AttackData>& attacks)
: Character(position, name, attacks)
, m_MoveDirection()
, m_attack(nullptr)
, m_damagePainFrames(0)
, m_playerInSightLastFrame(false)
{
    m_MoveDirection = mathgp::vc(0.f, 0.0f, 0.f);
}

MonsterCharacter::~MonsterCharacter()
{
    delete m_attack;
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

void MonsterCharacter::GetDamage()
{
    //armor...

    m_damagePainFrames = 2;

    Character::GetDamage();
}

void MonsterCharacter::update(int dt)
{
    m_timeAtLastPosition += dt;

    //SetTargetPoint(World::instance().mainCharacter()->position());
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

    // determine stats
    m_size = 0.5f + Max_Monster_Size *
        dna(G_Size);

    // reduce speed by up to 40% based on size
    m_speed = Max_Monster_Speed *
        saturate(dna(G_Speed) - dna(G_Size) * 0.4f);
    m_regularSpeed = m_speed;
    m_aggroSpeed = m_speed * Aggro_Speed_Factor;

    // increase hp by up to 20% based on size
    m_hp = m_maxHp = int(Max_Monster_HP *
        saturate(dna(G_HP) + 0.2f * dna(G_Size)));

    // decrease stamina on small size
    m_stamina = m_maxStamina = int(Max_Monster_Stamina *
        saturate(dna(G_Stamina)));
    m_restCooldown = 0;
    m_neededRestTime = 500 + int(dna(G_Size) * Max_Rest_Time);

    m_chanceToAggroOnSight = World_Range_Factor * g_worldSize * dna(G_Aggresiveness);
    m_aggroCooldown = 0;
    m_aggroTime = int(dna(G_Aggresiveness) * Max_Aggro_Time); // 1 minute;

    m_sightRange = World_Range_Factor * g_worldSize * dna(G_Sight);
    m_hearingRange = World_Range_Factor * g_worldSize * dna(G_Hearing);

    m_randomAttackCooldown = m_timeToDecideForRandomAttack = 
        dna(G_AttackDesire) * Max_Random_Attack_Wait;

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

    m_aggroStaminaLoss = 0;

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
        m_attack = new RangedAttack;
        m_attack->m_range = Base_Spitter_Range * m_dna(G_SpitterPower) + (Base_Spitter_Range * m_dna(G_Size)) / 2;
        m_attack->m_aoeRange = Spitter_AOE_Radius;
        m_attack->m_damage = int(Base_Spitter_Damage * m_dna(G_SpitterPower));
        m_attack->m_cooldownTime = int(Base_Spitter_Cooldown - (Base_Spitter_Cooldown * m_dna(G_Size)) / 2);
        m_attack->m_projectileSpeed = Base_Spitter_Projectile_Speed + (Base_Spitter_Range * m_dna(G_Size)) / 2;
        m_attack->m_staminaCostPerHit = Base_Spitter_Stamina_Cost + (Base_Spitter_Stamina_Cost * m_dna(G_SpitterPower)) / 2;
        break;
    case G_UseGrapple:
        m_attack = new MeleeAttack;
        m_attack->m_range = Base_Grapple_Range * m_dna(G_GrapplePower) + (Base_Grapple_Range * m_dna(G_Size)) / 2;
        m_attack->m_aoeRange = Grapple_AOE_Radius;
        m_attack->m_damage = int(Base_Grapple_Damage * m_dna(G_GrapplePower));
        m_attack->m_cooldownTime = int(Base_Grapple_Cooldown - (Base_Grapple_Cooldown * m_dna(G_Size)) / 2);
        m_attack->m_projectileSpeed = Base_Grapple_Projectile_Speed + (Base_Grapple_Range * m_dna(G_Size)) / 2;
        m_attack->m_staminaCostPerHit = Base_Grapple_Stamina_Cost + (Base_Grapple_Stamina_Cost * m_dna(G_GrapplePower)) / 2;
        break;
    case G_UseClaws:
        m_attack = new MeleeAttack;
        m_attack->m_range = Base_Claws_Range * m_dna(G_ClawsPower) + (Base_Claws_Range * m_dna(G_Size)) / 2;
        m_attack->m_aoeRange = Claws_AOE_Radius;
        m_attack->m_damage = int(Base_Claws_Damage * m_dna(G_ClawsPower));
        m_attack->m_cooldownTime = int(Base_Claws_Cooldown - (Base_Claws_Cooldown * m_dna(G_Size)) / 2);
        m_attack->m_projectileSpeed = Base_Claws_Projectile_Speed + (Base_Claws_Range * m_dna(G_Size)) / 2;
        m_attack->m_staminaCostPerHit = Base_Claws_Stamina_Cost + (Base_Claws_Stamina_Cost * m_dna(G_ClawsPower)) / 2;
        break;
    case G_UseThorns:
        m_attack = new RangedAttack;
        m_attack->m_range = Base_Thorns_Range * m_dna(G_ThornsPower) + (Base_Thorns_Range * m_dna(G_Size)) / 2;
        m_attack->m_aoeRange = Thorns_AOE_Radius;
        m_attack->m_damage = int(Base_Thorns_Damage * m_dna(G_ThornsPower));
        m_attack->m_cooldownTime = int(Base_Thorns_Cooldown - (Base_Thorns_Cooldown * m_dna(G_Size)) / 2);
        m_attack->m_projectileSpeed = Base_Thorns_Projectile_Speed + (Base_Thorns_Range * m_dna(G_Size)) / 2;
        m_attack->m_staminaCostPerHit = Base_Thorns_Stamina_Cost + (Base_Thorns_Stamina_Cost * m_dna(G_ThornsPower)) / 2;
        break;
    default:
        // no weapon!
        m_attack = new NoAttack;
        break;
    }

    m_attack->setOwner(this);

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

point3 MonsterCharacter::randomPointInSight()
{
    auto point = vc(Util::Rnd11(), Util::Rnd11(), Util::Rnd11());

    point *= m_sightRange;
    point += position();
    return point;
}

void MonsterCharacter::think(int dt)
{
    if (isDead())
    {
        SetMoveDirection(Vec::zero);
        return; // duh
    }

    m_lifetime += dt;

    ////////////////////////////////////////////
    // do cooldowns

    // hp regen
    m_regenPer100ms += float(dt) / 100;
    while (m_regenPer100ms >= 1)
    {
        heal(1);
        --m_regenPer100ms;
    }

    // random attack
    m_randomAttackCooldown -= dt;
    if (m_randomAttackCooldown < 0)
    {
        m_randomAttackCooldown = m_timeToDecideForRandomAttack;

        if (m_dna(G_AttackDesire) < Util::Rnd01())
        {
            // bam! random attack
            if (m_hasLastKnownPlayerPosition)
            {
                m_attack->attack(m_lastKnownPlayerPosition, m_lastKnownPlayerPosition);
            }
            else
            {
                auto point = randomPointInSight();
                m_attack->attack(point, point);
            }
        }
    }


    // aggro
    if (hasAggro())
    {
        m_aggroCooldown -= dt;
        if (m_aggroCooldown <= 0) m_aggroCooldown = 0;
    }

    // stamina
    if (isTired())
    {
        m_restCooldown -= dt;
        if (m_restCooldown <= 0) m_restCooldown = 0;
    }

    if (isLoitering())
    {
        m_loiterCooldown -= dt;
        if (m_loiterCooldown <= 0) m_loiterCooldown = 0;
    }

    if (m_damagePainFrames > 0)
    {
        --m_damagePainFrames;
    }

    ////////////////////////////////////////////
    // senses
    see();

    hear();

    ////////////////////////////////////////////
    // behave
    if (isTired())
    {
        // we're resting
        SetMoveDirection(Vec::zero);
        return;
    }

    if (!hasAggro())
    {
        // no aggro -> stroll aimlessly around the world
        // or loiter

        m_speed = m_regularSpeed;

        if (isLoitering())
        {
            return;
        }

        if (m_hasPointToGoTo)
        {
            if (distance(position(), m_pointToGoTo) > Close_Distance_In_World)
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
            m_loiterCooldown = int(Util::Rnd01() * Max_Loiter_Time);
            m_hasPointToGoTo = false;
        }
        else
        {
            // choose point to go to in sight range
            auto point = randomPointInSight();            

            m_pointToGoTo = point;
            m_hasPointToGoTo = true;

            SetTargetPoint(point);
        }
    }
    else // we have aggro!
    {
        // lose stamina
        m_aggroStaminaLoss += dt;
        while (m_aggroStaminaLoss > Time_To_Lose_1_Stamina_In_Aggro)
        {
            m_aggroStaminaLoss -= Time_To_Lose_1_Stamina_In_Aggro;
            loseStamina(1);
        }        

        // check if the wretched player is within our sight

        m_speed = m_aggroSpeed;

        World& world = World::instance();
        MainCharacter* enemy = world.mainCharacter();

        float distanceToPlayer = distance(position(), enemy->position());

        if (distanceToPlayer < m_sightRange)
        {
            // we see him
            // go towards him
            SetTargetPoint(enemy->position());

            if (distanceToPlayer < m_attack->senseOfRange())
            {
                // we think we can also attack
                m_attack->attack(enemy->position(), enemy->position());
            }
            
            // should we only care about this when in aggro?
            m_hasLastKnownPlayerPosition = true;
            m_lastKnownPlayerPosition = enemy->position();
        }
        else
        {
            if (m_hasLastKnownPlayerPosition)
            {
                if (distance(position(), m_lastKnownPlayerPosition) > Close_Distance_In_World)
                {
                    // go there
                    SetTargetPoint(m_lastKnownPlayerPosition);
                    return;
                }
            }

            // roam
            SetTargetPoint(randomPointInSight());
        }
    }
}

void MonsterCharacter::aggravate()
{
    m_aggroCooldown = m_aggroTime;

    // remove some meaninless stuff now
    m_hasPointToGoTo = false;
}

void MonsterCharacter::loseStamina(int n)
{
    m_stamina -= n;
    if (m_stamina <= 0)
    {
        m_stamina = 0;
        m_restCooldown = m_neededRestTime;
    }
}

void MonsterCharacter::see()
{
    std::vector<std::shared_ptr<Object> > objects = ColliderGrid::instance().collideWithCircle(mathgp::vc(m_pos.x(), m_pos.y()), m_sightRange);

    bool playerNear = false;

    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        if ((*it)->type() == EMonster_Character && (*it).get() != this)
        {
            MonsterCharacter* monster = (MonsterCharacter*)((*it).get());

            if (monster->m_damagePainFrames > 0)
            {
                aggravate();
            }
        }
        
        if ((*it)->type() == EPlayer_Character)
        {
            playerNear = true;
            if (!m_playerInSightLastFrame)
            {
                aggravate();
            }
        }
    }

    m_playerInSightLastFrame = playerNear;
}

void MonsterCharacter::hear()
{
    std::vector<std::shared_ptr<Object> > objects = ColliderGrid::instance().collideWithCircle(mathgp::vc(m_pos.x(), m_pos.y()), m_hearingRange);

    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        if ((*it)->type() != EMonster_Character || (*it).get() == this)
        {
            continue;
        }

        MonsterCharacter* monster = (MonsterCharacter*)((*it).get());

        if (monster->hasAggro())
        {
            aggravate();

            break;
        }
    }
}

float MonsterCharacter::fitness() const
{
    return m_damageDealtToPlayer * (float(m_lifetime) / 1000);
}

