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
#include "World.h"
#include "Bullet.h"
#include "Object.h"
#include "ColliderGrid.h"
#include "StaticObject.h"
#include "MainCharacter.h"
#include "MonsterCharacter.h"


World::World()
    : m_firstFreeId(0)
    , m_mainCharacter(nullptr)
{
}

World::~World()
{
}

std::shared_ptr<Object> World::object(unsigned int id)
{
	std::map<unsigned int, std::shared_ptr<Object> >::iterator it = m_objects.find(id);
	if (it != m_objects.end())
	{
		return it->second;
	}
	return NULL;
}

unsigned int World::spawnObject(float x, float y, float r)
{
	mathgp::vector3 pos = mathgp::v(x, y, 0.0f);	
    unsigned int id = m_firstFreeId;
    std::shared_ptr<Object> object(new Object(pos, r));
    object->id() = id;
    object->type(EBase_Object);
    m_objects[id] = object;
    ColliderGrid::instance().onObjectCreated(m_objects[id]);

	++m_firstFreeId;

	if (m_firstFreeId >= INT_MAX)
	{
		m_firstFreeId = 0; //hopefully his dead;
	}

    return id;
}

unsigned int World::spawnMonster(float x, float y, float r, const std::string& name, const AttacksData& attacks)
{
    mathgp::vector3 pos = mathgp::v(x, y, 0.0f);
    unsigned int id = m_firstFreeId;

    MonsterCharacter* monster = new MonsterCharacter(pos, name, attacks);
    monster->id() = id;
    monster->type(EMonster_Character);
    monster->Move(pos);

    m_objects[id] = std::shared_ptr<Object>(monster);
    ColliderGrid::instance().onObjectCreated(m_objects[id]);

    ++m_firstFreeId;

    if (m_firstFreeId >= INT_MAX)
    {
        m_firstFreeId = 0; //hopefully his dead;
    }

    return id;
}

unsigned int World::spawnPlayer(float x, float y, float r, const AttacksData& attacks)
{
    mathgp::vector3 pos = mathgp::v(x, y, 0.0f);
    unsigned int id = m_firstFreeId;
    MainCharacter* player = new MainCharacter(pos, attacks);
    player->id() = id;
    player->type(EPlayer_Character);
    player->Move(pos);

    m_objects[id] = std::shared_ptr<Object>(player);
    ColliderGrid::instance().onObjectCreated(m_objects[id]);

    m_mainCharacter = player;

    ++m_firstFreeId;

    if (m_firstFreeId >= INT_MAX)
    {
        m_firstFreeId = 0; //hopefully his dead;
    }

    return id;
}

unsigned int World::spawnBullet(float x, float y, float r, SpritePtr projectile, SpritePtr impact, const mathgp::vector3& direction, float speed, float maxDistance)
{
    mathgp::vector3 pos = mathgp::v(x, y, 0.01f);
    unsigned int id = m_firstFreeId;
    Bullet* bullet = new Bullet(pos, r);

    bullet->init(projectile, impact, pos, direction, speed, maxDistance);

    bullet->id() = id;
    bullet->Move(pos);

    m_objects[id] = std::shared_ptr<Object>(bullet);
    ColliderGrid::instance().onObjectCreated(m_objects[id]);
    
    ++m_firstFreeId;

    if (m_firstFreeId >= INT_MAX)
    {
        m_firstFreeId = 0; //hopefully his dead;
    }

    return id;
}

unsigned int World::spawnStaticObject(float x, float y, float r, SpritePtr sprite)
{
    mathgp::vector3 pos = mathgp::v(x, y, 0.0f);
    unsigned int id = m_firstFreeId;
    StaticObject* object = new StaticObject(sprite, pos, r);
    object->id() = id;

    m_objects[id] = std::shared_ptr<Object>(object);
    ColliderGrid::instance().onObjectCreated(m_objects[id]);

    ++m_firstFreeId;

    if (m_firstFreeId >= INT_MAX)
    {
        m_firstFreeId = 0; //hopefully his dead;
    }

    return id;
}

void World::destroyObject(unsigned int id)
{
    ColliderGrid::instance().onObjectDestroyed(object(id));
    m_objects.erase(id);
}

objectsContainer& World::objects()
{
    return m_objects;
}

void World::queueObjectForDestruction(unsigned int id)
{
    m_ObjectsPendingDestruction.push_back(id);
}

void World::update(int dt)
{
    desetroyPendingObjects();

    for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
    {
        it->second->update(dt);
    }

    // deal monster damages
    for (auto i = m_monsterDamages.begin(); i != m_monsterDamages.end(); ++i)
    {
        i->remainingTime -= dt;

        if (i->remainingTime <= 0)
        {
            if (distance(i->position, m_mainCharacter->position()) < i->raidus + m_mainCharacter->r())
            {
                int dmgDealt = i->damage;
                m_mainCharacter->OnHit(EMonsterDamage, dmgDealt);

                // notify monster about his damage
                auto dealer = object(i->ownerId);
                if (dealer)
                {
                    assert(dealer->type() == EMonster_Character);
                    auto monster = static_cast<MonsterCharacter*>(dealer.get());
                    monster->onDealtDamage(dmgDealt);
                }
            }

            i = m_monsterDamages.erase(i);
            if (i == m_monsterDamages.end())
            {
                break;
            }
        }
    }

    desetroyPendingObjects();
}

void World::desetroyPendingObjects()
{
    for (auto it = m_ObjectsPendingDestruction.begin(); it != m_ObjectsPendingDestruction.end(); ++it)
    {
        destroyObject(*it);
    }

    m_ObjectsPendingDestruction.clear();
}

void World::registerMonsterDamage(const RegisteredMonsterDamage& damage)
{
    m_monsterDamages.push_back(damage);
}
