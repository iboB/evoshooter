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
#include "ColliderGrid.h"
#include "World.h"
#include "Object.h"
#include "Application.h"
#include "Camera.h"
#include "GameState.h"
#include "MainWindow.h"
#include <functional>

using namespace mathgp;

ColliderGrid::ColliderGrid():
m_sizeX(g_gridCells),
m_sizeY(g_gridCells),
m_grid(g_gridCells, std::vector<std::list<std::shared_ptr<Object> > >(g_gridCells, std::list<std::shared_ptr<Object> >()))
{
    
}

ColliderGrid::~ColliderGrid()
{
    m_dummyObject.reset();
}

void ColliderGrid::initialize()
{
    m_dummyObjectId = World::instance().spawnObject(-20, -20, 0); // dummy object
    m_dummyObject = World::instance().object(m_dummyObjectId);
}

std::shared_ptr<Object> ColliderGrid::requestMoveTo(Object* obj, const mathgp::vector2& pos)
{
	return requestMoveTo(obj, pos.x(), pos.y());
}
std::shared_ptr<Object> ColliderGrid::requestMoveTo(Object* obj, const mathgp::vector3& pos)
{
	return requestMoveTo(obj, pos.x(), pos.y());
}

std::vector<std::shared_ptr<Object> > ColliderGrid::collideWithCircle(mathgp::vector2 origin, float r)
{
    std::vector<std::shared_ptr<Object> > out;

    mathgp::uvector2 id = getObjectCell(origin.x(), origin.y());
    

    std::list< std::shared_ptr< Object > >::iterator it;
    int x, y;
    for (int i = -1; i <= 1; ++i)
    {
        x = id.x() + i;
        if (x < 0 || x >= m_sizeX)
        {
            continue;
        }
        for (int j = -1; j <= 1; ++j)
        {
            y = id.y() + j;

            if (y < 0 || y >= m_sizeY)
            {
                continue;
            }

            it = m_grid[x][y].begin();
            while (it != m_grid[x][y].end())
            {
                if (std::sqrt(((*it)->x() - origin.x()) * ((*it)->x() - origin.x()) + ((*it)->y() - origin.y()) * ((*it)->y() - origin.y())) < r + (*it)->r())
                {
                    out.push_back((*it));
                }
                ++it;
            }
        }
    }

    return out;
}

std::shared_ptr<Object> ColliderGrid::requestMoveTo(Object* obj, float newX, float newY)
{
    if (newY >= (g_worldSize - g_worldBorderOffset) || newY <= g_worldBorderOffset || newX >= (g_worldSize - g_worldBorderOffset) || newX <= g_worldBorderOffset)
    {
        //disallow move, return dummy obj, use isWall to test;
        return m_dummyObject;
    }

    mathgp::uvector2 oldId = getObjectCell(obj);
    mathgp::uvector2 id = getObjectCell(newX, newY);
    std::list< std::shared_ptr< Object > >::iterator it;
    int x, y;
    for (int i = -1; i <= 1; ++i)
    {
        x = id.x() + i;
        if (x < 0 || x >= m_sizeX)
        {
            continue;
        }
        for (int j = -1; j <= 1; ++j)
        {
            y = id.y() + j;

            if (y < 0 || y >= m_sizeY)
            {
                continue;
            }
                        
            it = m_grid[x][y].begin();
            while (it != m_grid[x][y].end())
            {
                if ((*it).get() != obj)
                {                    
                    if (std::sqrt(((*it)->x() - newX) * ((*it)->x() - newX) + ((*it)->y() - newY) * ((*it)->y() - newY)) < obj->r() + (*it)->r())
                    {
                        return *it;
                    }
                }
                ++it;
            }
        }
    }

    obj->x(newX);
    obj->y(newY);

    if (oldId != id)
    {
        bool found = false;
        for (auto it2 = m_grid[oldId.x()][oldId.y()].begin(); it2 != m_grid[oldId.x()][oldId.y()].end(); ++it2)
        {
            if((*it2).get() == obj)
            {
                it = it2;
                found = true;
                break;
            }
        }

        if (!found)
        {
            assert(false);

            return NULL;
        }

        //it = std::find(m_grid[oldId.x()][oldId.y()].begin(), m_grid[oldId.x()][oldId.y()].end(), obj);
        std::shared_ptr<Object> objToMove = *it;
        m_grid[oldId.x()][oldId.y()].erase(it);
        m_grid[id.x()][id.y()].push_back(objToMove);
    }

    return NULL;
}

void ColliderGrid::onObjectCreated(std::shared_ptr<Object> obj)
{
    if (m_dummyObject != obj)
    {
        mathgp::uvector2 id = getObjectCell(obj.get());
        m_grid[id.x()][id.y()].push_back(obj);
    }
}
void ColliderGrid::onObjectDestroyed(std::shared_ptr<Object> obj)
{
    mathgp::uvector2 id = getObjectCell(obj.get());
    std::list< std::shared_ptr< Object > >::iterator it = std::find(m_grid[id.x()][id.y()].begin(), m_grid[id.x()][id.y()].end(), obj);
    if (it != m_grid[id.x()][id.y()].end())
    {
        m_grid[id.x()][id.y()].erase(it);
    }
}

mathgp::uvector2 ColliderGrid::getObjectCell(Object* obj)
{
//#ifndef _DEBUG
//	return mathgp::v((unsigned int)(obj->x() / g_gridSize), (unsigned int)(obj->y() / g_gridSize));
//#else
    return cullIdToBounds(mathgp::v((unsigned int)(obj->x() / g_gridSize), (unsigned int)(obj->y() / g_gridSize)));
//#endif
}

mathgp::uvector2 ColliderGrid::getObjectCell(float x, float y)
{
//#ifndef _DEBUG
//    return mathgp::v((unsigned int)(x / g_gridSize), (unsigned int)(y / g_gridSize));
//#else
    return cullIdToBounds(mathgp::v((unsigned int)(x / g_gridSize), (unsigned int)(y / g_gridSize)));
//#endif
}

mathgp::uvector2& ColliderGrid::cullIdToBounds(mathgp::uvector2& id)
{
    if (id.x() < 0)
        id.x() = 0;
    else if (id.x() >= (unsigned int)m_sizeX)
        id.x() = m_sizeX - 1;

    if (id.y() < 0)
        id.y() = 0;
    else if (id.y() >= (unsigned int)m_sizeY)
        id.y() = m_sizeY - 1;

    return id;
}

struct SortComparer
{
    SortComparer(ColliderGrid* g) : grid(g) {}

    ColliderGrid* grid;

    bool operator()(const std::shared_ptr<Object> obj1, const std::shared_ptr<Object> obj2)
    {
        return grid->sortCompare(obj1, obj2);
    }
};

bool ColliderGrid::sortCompare(const std::shared_ptr<Object> obj1, const std::shared_ptr<Object> obj2)
{
    return ((obj1->position() - m_currentCollisionRayStart).length_sq() <= (obj2->position() - m_currentCollisionRayStart).length_sq());
}
std::vector<std::shared_ptr<Object> > ColliderGrid::collideCirclesWith2dRay(mathgp::vector2 start, mathgp::vector2 end)
{
    std::vector<std::shared_ptr<Object> > out;
    m_currentCollisionRayStart = v(start.x(), start.y(), 0.0f);
    mathgp::vector2 vec = end - start;
    vec.normalize();
    vec *= g_worldSize;

    mathgp::vector2 d = vec;
    mathgp::vector2 f;
    mathgp::vector2 objPos2d;
    std::shared_ptr<Object> obj;

    objectsContainer& objects = World::instance().objects();
    objectsContainer::iterator it = objects.begin();

    while (it != objects.end())
    {
        obj = it->second;
        it++;
        objPos2d.x() = obj->x();// +(obj->r() / 2);
        objPos2d.y() = obj->y();// +(obj->r() / 2);
        f = start - objPos2d;
        float a = d.length_sq();
        //mathgp::vector2 dd;
        //mathgp::_internal::dot(dd, dd);
        float b = 2 * mathgp::_internal::dot(f, d);
        float c = f.length_sq() - obj->r() * obj->r();
        float discriminant = b*b - 4 * a*c;

        if (discriminant < 0)
        {
            continue;
        }
        else
        {
            discriminant = std::sqrt(discriminant);
            float t1 = (-b - discriminant) / (2 * a);
            float t2 = (-b + discriminant) / (2 * a);

            if (t1 >= 0 && t1 <= 1)
            {
                //impale and poke are here
                out.push_back(obj);
            }

            continue;
            if (t2 >= 0 && t2 <= 1)
            {
                //exit wound, we don't need that
            }
        }
    }

    std::sort(out.begin(), out.end(), SortComparer(this));

    return out;
}
//#include <iostream>
std::vector<std::shared_ptr<Object> > ColliderGrid::collideWithQuadsOnClick(const mathgp::uvector2& screenPos, const mathgp::vector3& worldPoint)
{
    m_currentCollisionRayStart = v(0.0f, 0.0f, 0.0f); //todo: make this player pos;
    const vector3 up = v(0.0f, 0.0f, 1.0f);
    const vector3 left = v(-1.0f, 0.0f, 0.0f);
    std::vector<std::shared_ptr<Object> > out;
    uvector2 id = getObjectCell(worldPoint.x(), worldPoint.y());
    vector3 topLeftWorld;
    vector3 botRightWorld;
    vector3 topLeftScreen;
    vector3 botRightScreen;
    
    const uvector2& screenSize = Application::instance().mainWindow()->clientAreaSize();

    uvector2 halfScreenSize = v(screenSize.x() / 2, screenSize.y() / 2);
   // vector2 ooScreenCoords = v(float((int)screenPos.x() - (int)halfScreenSize.x()) / float(halfScreenSize.x()), float((int)halfScreenSize.y() - (int)screenPos.y()) / float(halfScreenSize.y()));
    
    Camera* cam = Application::instance().currentState()->camera();
    const matrix& projectionView = cam->projectionView();
    //traverse all objects in the cell and all adjacent cells;
    std::list< std::shared_ptr< Object > >::iterator it;
    int x, y;
    for (int i = -1; i <= 1; ++i)
    {
        x = id.x() + i;       
        if (x < 0 || x >= m_sizeX)
        {
            continue;
        }
        for (int j = -1; j + id.y() <= 1; ++j)
        {
            y = id.y() + j;
            if (y < 0 || y >= m_sizeY)
            {
                continue;
            }
           
            it = m_grid[x][y].begin();
            while (it != m_grid[x][y].end())
            {
                topLeftWorld = (*it)->position() + (up*(*it)->bb_h()) + (left*((*it)->bb_w() / 2.0f));
                botRightWorld = (*it)->position() + ((-1.0f*left)*((*it)->bb_w() / 2.0f));

                topLeftScreen = transform_coord(topLeftWorld, projectionView);
                
                topLeftScreen.x() = (topLeftScreen.x()*halfScreenSize.x()) + halfScreenSize.x();
                topLeftScreen.y() = halfScreenSize.y() - (topLeftScreen.y()*halfScreenSize.y());

                botRightScreen = transform_coord(botRightWorld, projectionView);
                
                botRightScreen.x() = (botRightScreen.x()*halfScreenSize.x()) + halfScreenSize.x();
                botRightScreen.y() = halfScreenSize.y() - (botRightScreen.y()*halfScreenSize.y());

                if (screenPos.x() >= topLeftScreen.x() && screenPos.x() <= botRightScreen.x() &&
                    screenPos.y() >= topLeftScreen.y() && screenPos.y() <= botRightScreen.y())
                {
                    //std::cout << "COLLIDED WITH obj on pos x : " << (*it)->x();
                    out.push_back(*it);
                }
                ++it;
            }

        }
    }

    std::sort(out.begin(), out.end(), SortComparer(this));

    return out;
}
/*
bool ColliderGrid::collideAABBWith3dRay(const std::shared_ptr<Object> obj, const mathgp::vector3& rayStart, const mathgp::vector3& rayDir)
{
    bool inside = true;
   // vector3 AABBcenter = obj->position() + (obj->bb_h() / 2.0f);
   // vector3 minB = obj->position
    return true;
}
*/