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

#include "Object.h"

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;

class Bullet : public Object
{
public:
    Bullet(const mathgp::vector3& pos, float bc);
    ~Bullet();

    void init(SpritePtr projectile, SpritePtr impact, const mathgp::vector3& startPosition, const mathgp::vector3& direction, float speed, float maxDistance);
    
    void shoot();
    
    virtual void update(int dt);

    void setDamageType(EAttackDamageType in) { m_damageType = in; }
    void setDamage(int in) { m_damage = in; }
private:

    SpritePtr m_Projectile;
    SpritePtr m_Impact;

    mathgp::vector3 m_StartPosition;
    mathgp::vector3 m_Direction;
    float m_Speed;
    float m_MaxDistanceSq;

    bool m_IsFlying;
    bool m_IsExploding;

    EAttackDamageType m_damageType;
    int m_damage;
};
