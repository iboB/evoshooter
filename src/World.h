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

#include "ExplicitSingleton.h"

#include "MonsterCharacter.h"
#include "RegisteredMonsterDamage.h"

class Object;
class MainCharacter;

const float g_worldSize = 39.0f;
const float g_gridSize = 5.0f;
const float g_worldBorderOffset = 0.2f; //distance of world border you are not allowed to go;

typedef std::map<unsigned int, std::shared_ptr<Object> > objectsContainer;

const unsigned int g_gridCells = (const unsigned int)(g_worldSize / g_gridSize) + 1;
class World : public ExplicitSingleton<World>
{
    DECLARE_EXPLICIT_SINGLETON(World);
public:

	std::shared_ptr<Object> object(unsigned int id);
    unsigned int spawnObject(float x, float y, float r);
    unsigned int spawnMonster(float x, float y, float r, const std::string& name, const AttacksData& attacks);
    unsigned int spawnPlayer(float x, float y, float r, const AttacksData& attacks);
    unsigned int spawnBullet(float x, float y, float r, SpritePtr projectile, SpritePtr impact, const mathgp::vector3& direction, float speed, float maxDistance);
    unsigned int spawnStaticObject(float x, float y, float r, SpritePtr sprite);
    void destroyObject(unsigned int id);
    objectsContainer& objects();
    MainCharacter* mainCharacter() { return m_mainCharacter; }

    void queueObjectForDestruction(unsigned int id);

    void update(int dt);

    void registerMonsterDamage(const RegisteredMonsterDamage& damage);

private:
    void desetroyPendingObjects();

    objectsContainer m_objects;
    MainCharacter* m_mainCharacter;
	unsigned int m_firstFreeId;

    std::vector<unsigned int> m_ObjectsPendingDestruction;

    std::list<RegisteredMonsterDamage> m_monsterDamages;

    bool m_update;
};
