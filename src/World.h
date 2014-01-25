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

const float g_worldSize = 50.0f;
const float g_gridSize = 5.0f;
const unsigned int g_gridCells = (const unsigned int)(g_worldSize / g_gridSize);
class World : public ImplicitSingleton<World>
{
public:
	World();
	~World();

	std::shared_ptr<Object> object(unsigned int id);
	unsigned int spawnObject(float x, float y, float r);
    void destroyObject(unsigned int id);
	
private:
	std::map<unsigned int, std::shared_ptr<Object> > m_objects;
	unsigned int m_firstFreeId;
};
