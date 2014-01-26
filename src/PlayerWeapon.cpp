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
#include "PlayerWeapon.h"
#include "World.h"
#include "ColliderGrid.h"
#include "MainCharacter.h"
#include "Util.h"
#include "Bullet.h"
#include "ResourceManager.h"

std::string PlayerWeapon::m_weaponNames[EWeaponCount] =
{
    "Knife", "Gun", "Shotgun"
};
EAttackDamageType PlayerWeapon::m_weaponDamageTypes[EWeaponCount] =
{
    EPiercing, EBlunt, EBlunt
};
EAttackType PlayerWeapon::m_weaponAttackTypes[EWeaponCount] =
{
    EMele, EShoot, EShoot
};

using namespace mathgp;

PlayerWeapon::PlayerWeapon(EWeaponType t, unsigned attackDelay, const uvector2& dmgRange):
m_type(t)
{
    m_attackType = m_weaponAttackTypes[m_type];
    m_damageType = m_weaponDamageTypes[m_type];
    m_attackDelay = attackDelay;
    m_lastAttackTimestamp = 0;
    m_damageRange = dmgRange;
}
PlayerWeapon::~PlayerWeapon()
{
}

void PlayerWeapon::attack(const mathgp::vector3& worldPoint, Object* objectHit)
{

    unsigned int now = SDL_GetTicks();
    if (now >= m_lastAttackTimestamp + m_attackDelay)
    {
        now = SDL_GetTicks();
        switch (m_type)
        {
        case EKnife:
            meleeAttack(worldPoint);
            break;
        case EPistol:
            {
                if (objectHit && objectHit->type() != EPlayer_Character)
                {
                    rangedGunAttack(objectHit->position());
                }
                else
                {
                    rangedGunAttack(worldPoint);
                }
            }
            break;
        case EShotgun:
        {
            {
                if (objectHit && objectHit->type() != EPlayer_Character)
                {
                    rangedShotgunAttack(objectHit->position());
                }
                else
                {
                    rangedShotgunAttack(worldPoint);
                }
            }
        }
            break;
        }
    }
}
unsigned PlayerWeapon::damage()
{
    return (unsigned)((m_damageRange.y() - m_damageRange.x())*Util::Rnd01() + m_damageRange.x());
}
void PlayerWeapon::meleeAttack(const mathgp::vector3& worldPoint)
{
    vector3 playerPos = World::instance().mainCharacter()->position();
    vector3 directionOfAttack = normalized(worldPoint - playerPos);
    vector3 pointOfAttack = directionOfAttack * 0.4f + playerPos;

    std::vector< std::shared_ptr<Object> > affectedTargets = ColliderGrid::instance().collideWithCircle(pointOfAttack.xy(), 0.5f);
    std::vector< std::shared_ptr<Object> >::iterator it = affectedTargets.begin();
    while (it != affectedTargets.end())
    {
       
        (*it)->OnHit(m_damageType, damage());
        ++it;
    }
}

void PlayerWeapon::rangedShotgunAttack(const mathgp::vector3& worldPoint)
{
    SpritePtr projectile = ResourceManager::instance().createSpriteFromSingleFrameTexture("sprites/projectiles/rocket.png");
    SpritePtr impact = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/projectiles/explosion.png", 1, 4, 400);
    vector3 playerPos = World::instance().mainCharacter()->position();
    vector3 directionOfAttack = normalized(worldPoint - playerPos);

    unsigned int id = World::instance().spawnBullet(playerPos.x(), playerPos.y(), 0.1f, projectile, impact, directionOfAttack, 3.f, 5.f);
    Bullet* bullet = (Bullet*)(World::instance().object(id).get());
    bullet->setDamage(damage());
    bullet->setDamageType(m_damageType);
    bullet->shoot();
}
void PlayerWeapon::rangedGunAttack(const mathgp::vector3& worldPoint)
{
    SpritePtr projectile = ResourceManager::instance().createSpriteFromSingleFrameTexture("sprites/projectiles/rocket.png");
    SpritePtr impact = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/projectiles/explosion.png", 1, 4, 400);
    vector3 playerPos = World::instance().mainCharacter()->position();
    vector3 directionOfAttack = normalized(worldPoint - playerPos);

    unsigned int id = World::instance().spawnBullet(playerPos.x(), playerPos.y(), 0.1f, projectile, impact, directionOfAttack, 3.f, 5.f);
    Bullet* bullet = (Bullet*)(World::instance().object(id).get());
    bullet->setDamage(damage());
    bullet->setDamageType(m_damageType);
    bullet->shoot();
}