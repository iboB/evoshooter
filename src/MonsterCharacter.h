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

#include "Character.h"
#include "MonsterDNA.h"

class MonsterAttack;

enum EMonsterDefenseType
{
    ENone = 0,
    EScales,
    EFatness
};

class MonsterCharacter : public Character
{
public:
    MonsterCharacter(const mathgp::vector3& position, const std::string& name, const std::vector<AttackData>& attacks);
    ~MonsterCharacter();

    void SetMoveDirection(const mathgp::vector3& dir);
    void SetTargetPoint(const mathgp::point3& point);

    virtual void GetDamage();

    virtual void update(int dt);

    const MonsterDNA& dna() const { return m_dna;  }

    int isDead() { return m_hp <= 0; }

    void loseStamina(int n);

private:
    mathgp::vector3 m_MoveDirection;
    
    ///////////////////////////
    // evo stuff
public:
    void useDNA(const MonsterDNA& dna);
    MonsterDNA giveOffspring();

    float calculateFitness() const; // damage * lifetime

    bool hasAggro() { return m_aggroCooldown != 0; }

private:
    void think(int dt);
    MonsterDNA m_dna;

    void aggravate();

    // stats
    int m_maxHp;
    int m_hp;
    void heal(int hp);

    int m_maxStamina;
    int m_stamina;
    int m_restCooldown;
    int m_neededRestTime; // depends on size

    bool isTired() const { return m_restCooldown != 0; }
    
    float m_speed; // m/s
    float m_regularSpeed;
    float m_aggroSpeed;

    float m_randomAttackCooldown;
    float m_currentDesireToRandomAttack;

    float m_size;

    float m_chanceToAggroOnSight;
    int m_aggroCooldown;
    int m_aggroTime;

    void see();
    void hear();

    float m_sightRange;
    float m_hearingRange;

    float m_regenPer100ms;
    
    // fitness
    float m_damageDealtToPlayer;
    float m_lifetime;

    // memory
    bool m_hasLastKnownPlayerPosition;
    mathgp::point3 m_lastKnownPlayerPosition;
    bool m_playerInSightLastFrame;

    bool isLoitering() const { return m_loiterCooldown != 0; };
    int m_loiterCooldown;
    bool m_hasPointToGoTo;
    mathgp::point3 m_pointToGoTo;

    mathgp::point3 m_lastOwnPosition;
    int m_timeAtLastPosition;

    MonsterAttack* m_attack;

    int m_damagePainFrames;

    float m_defenseStrength;
    EMonsterDefenseType m_defenseType;
};
