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

#include "Bullet.h"

#include "ResourceManager.h"
#include "Sprite.h"

Bullet::Bullet()
: m_Speed(0.f)
, m_IsFlying(false)
, m_IsExploding(false)
{
    
}

Bullet::~Bullet()
{

}

void Bullet::init(const std::string& projectile, Uint32 projectileFrames, Uint32 projectileAnimDuration,
    const std::string& impact, Uint32 impactFrames, Uint32 impactAnimDuration,
    const mathgp::vector3& startPosition, const mathgp::vector3& direction, float speed)
{
    if (projectileFrames)
    {
        m_Projectile = ResourceManager::instance().createSpriteFromSingleAnimationTexture(projectile, 1, projectileFrames, projectileAnimDuration);
        m_Projectile->setLoop(true);
    }
    else
    {
        m_Projectile = ResourceManager::instance().createSpriteFromSingleFrameTexture (projectile);
    }

    if (projectileFrames)
    {
        m_Impact = ResourceManager::instance().createSpriteFromSingleAnimationTexture(impact, 1, impactFrames, impactAnimDuration);
    }
    else
    {
        m_Impact = ResourceManager::instance().createSpriteFromSingleFrameTexture(impact);
    }

    m_Direction = direction;
    m_Speed = speed;
}

void Bullet::shoot()
{
    m_IsFlying = true;
}

void Bullet::update(int dt)
{
    if (!m_IsFlying)
    {
        return;
    }
}
