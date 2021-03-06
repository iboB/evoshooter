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
#include "World.h"
#include "Bullet.h"
#include "Object.h"
#include "ColliderGrid.h"
#include "StaticObject.h"
#include "MainCharacter.h"
#include "MonsterCharacter.h"
#include "Util.h"

World::World()
    : m_firstFreeId(0)
    , m_mainCharacter(nullptr)
    , m_update(false)
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

int numspawned;

//unsigned int World::spawnMonster(float x, float y, float r, const std::string& name, const AttacksData& attacks)
unsigned int World::spawnMonster(const MonsterDNA& dna)
{
    ++numspawned;
    std::cout << "spawned " << numspawned << std::endl;
    for (int i = 0; i < Num_Genes; ++i)
    {
        std::cout << dna.m[i].value() << "\n";
    }

    
    float x = Util::Rnd01(), y = Util::Rnd01();
    x *= g_worldSize-1; y *= g_worldSize-1;
    mathgp::vector3 pos = mathgp::v(x, y, 0.0f);
    unsigned int id = m_firstFreeId;

    MonsterCharacter* monster = new MonsterCharacter(pos, dna);
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
    bullet->type(EBullet);
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
    mathgp::vector3 pos = mathgp::v(x, y, -0.4f);
    unsigned int id = m_firstFreeId;
    StaticObject* object = new StaticObject(sprite, pos, r);
    object->type(EStatic);
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
    if (m_update)
    {
        queueObjectForDestruction(id);
        return;
    }
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
    m_update = true;
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
                std::cout << "Hitting player, from :" << i->ownerId << " @" << SDL_GetTicks() << std::endl;
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
    m_update = false;
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

void World::reset()
{
    m_objects.clear();

    m_monsterDamages.clear();
    ColliderGrid::instance().resest();
    m_firstFreeId = 0;
}
