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
#include "Application.h"
#include "Camera.h"
#include "GameState.h"
#include "ColliderGrid.h"
#include "World.h"

Bullet::Bullet(const mathgp::vector3& pos, float bc)
: Object(pos, bc)
, m_Speed(0.f)
, m_IsFlying(false)
, m_IsExploding(false)
, m_MaxDistanceSq(0)
{
    m_damage = 0;
    m_damageType = EMonsterDamage;
}

Bullet::~Bullet()
{

}

void Bullet::init(SpritePtr projectile, SpritePtr impact, const mathgp::vector3& startPosition, const mathgp::vector3& direction, float speed, float maxDistance)
{
    m_Projectile = projectile;
    m_Projectile->setScale(0.005f);
    m_Impact = impact;
    if (m_Impact)
    {
        m_Impact->setLoop(false);
        m_Impact->setScale(0.005f);
    }
    m_StartPosition = startPosition;
    m_Direction = direction;
    if (m_Direction != Vec::zero)
    {
        m_Direction.normalize();
    }
    m_Speed = speed;
    m_MaxDistanceSq = maxDistance * maxDistance;
}

void Bullet::shoot()
{
    m_IsFlying = true;

    m_Projectile->startRendering(0);
}

void Bullet::update(int dt)
{
    if (!m_IsFlying)
    {
        return;
    }

    if (m_IsExploding)
    {
        m_Impact->update(m_pos, Application::instance().currentState()->camera()->direction());
        if (!m_Impact->isRendering())
        {
            World::instance().queueObjectForDestruction(m_id);
        }
        return;
    }

    mathgp::vector3 offset = m_Direction * (m_Speed * float(dt) * 0.001f);

    std::shared_ptr<Object> collision = ColliderGrid::instance().requestMoveTo(this, m_pos + offset);

    if (!collision)
    {
        if ((m_pos - m_StartPosition).length_sq() > m_MaxDistanceSq)
        {
            World::instance().queueObjectForDestruction(m_id);
            return;
        }
        
        m_Projectile->update(m_pos, Application::instance().currentState()->camera()->direction());

        return;
    }

    collision->GetDamage();
    collision->OnHit(m_damageType, m_damage);

    m_Projectile->stopRendering();
    
    if (m_Impact)
    {
        m_Impact->startRendering(0);
        m_IsExploding = true;
    }
    else
    {
        World::instance().queueObjectForDestruction(m_id);
    }
}
