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
    MonsterCharacter(const mathgp::vector3& position, const MonsterDNA& dna);
    ~MonsterCharacter();

    void SetMoveDirection(const mathgp::vector3& dir);
    void SetTargetPoint(const mathgp::point3& point);

    virtual void GetDamage();

    virtual void update(int dt);

    const MonsterDNA& dna() const { return m_dna;  }

    virtual void OnHit(EAttackDamageType dmgType, int dmg);

    void loseStamina(int n);

    void onDealtDamage(int dmgDealt) { m_damageDealtToPlayer += dmgDealt; }

    float fitness() const;

    void Die() override;

private:
    mathgp::vector3 m_MoveDirection;
    
    ///////////////////////////
    // evo stuff
public:
    void useDNA(const MonsterDNA& dna);
    MonsterDNA giveOffspring();


    bool hasAggro() { return m_aggroCooldown != 0; }

private:
    void think(int dt);
    MonsterDNA m_dna;

    void aggravate();

    // stats
    

    int m_maxStamina;
    int m_stamina;
    int m_restCooldown;
    int m_neededRestTime; // depends on size

    bool isTired() const { return m_restCooldown != 0; }
    
    float m_speed; // m/s
    float m_regularSpeed;
    float m_aggroSpeed;

    float m_randomAttackCooldown;
    float m_timeToDecideForRandomAttack;

    float m_size;

    float m_chanceToAggroOnSight;
    int m_aggroCooldown;
    int m_aggroTime;

    void see();
    void hear();

    float m_sightRange;
    mathgp::point3 randomPointInSight();
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

    float m_aggroStaminaLoss;

    float m_defenseStrength;
    EMonsterDefenseType m_defenseType;

    int applyArmor(int dmg);
};
