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
#include "Object.h"
#include "ColliderGrid.h"
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
    m_objects[id] = std::shared_ptr<Object>(new Object(pos, r));
    ColliderGrid::instance().onObjectCreated(m_objects[id]);

	++m_firstFreeId;

	if (m_firstFreeId >= INT_MAX)
	{
		m_firstFreeId = 0; //hopefully his dead;
	}

    return id;
}

unsigned int World::spawnMonster(float x, float y, float r, const std::string& name)
{
    mathgp::vector3 pos = mathgp::v(x, y, 0.0f);
    unsigned int id = m_firstFreeId;
    MonsterCharacter* monster = new MonsterCharacter(pos, name);
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

void World::destroyObject(unsigned int id)
{
    ColliderGrid::instance().onObjectDestroyed(object(id));
    m_objects.erase(id);
}

objectsContainer& World::objects()
{
    return m_objects;
}
