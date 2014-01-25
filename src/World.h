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

#include "ImplicitSingleton.h"

class Object;
class MainCharacter;

const float g_worldSize = 50.0f;
const float g_gridSize = 5.0f;

typedef std::map<unsigned int, std::shared_ptr<Object> > objectsContainer;

const unsigned int g_gridCells = (const unsigned int)(g_worldSize / g_gridSize);
class World : public ImplicitSingleton<World>
{
public:
	World();
	~World();

	std::shared_ptr<Object> object(unsigned int id);
    unsigned int spawnObject(float x, float y, float r);
    unsigned int spawnMonster(float x, float y, float r, const std::string& name);
    void destroyObject(unsigned int id);
    objectsContainer& objects();
    const MainCharacter* mainCharacter() { return m_mainCharacter; }
	
private:
    objectsContainer m_objects;
    MainCharacter* m_mainCharacter;
	unsigned int m_firstFreeId;
};
