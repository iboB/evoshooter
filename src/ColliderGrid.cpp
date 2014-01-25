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
#include <math.h>

using namespace mathgp;

ColliderGrid::ColliderGrid():
m_sizeX(g_gridCells),
m_sizeY(g_gridCells),
m_grid(g_gridCells, std::vector<std::list<std::shared_ptr<Object> > >(g_gridCells, std::list<std::shared_ptr<Object> >()))
{
}

ColliderGrid::~ColliderGrid()
{
}


std::shared_ptr<Object> ColliderGrid::requestMoveTo(std::shared_ptr<Object> obj, const mathgp::vector2& pos)
{
	return requestMoveTo(obj, pos.x(), pos.y());
}
std::shared_ptr<Object> ColliderGrid::requestMoveTo(std::shared_ptr<Object> obj, const mathgp::vector3& pos)
{
	return requestMoveTo(obj, pos.x(), pos.y());
}
std::shared_ptr<Object> ColliderGrid::requestMoveTo(std::shared_ptr<Object> obj, float newX, float newY)
{
    mathgp::uvector2 oldId = getObjectCell(obj);
    mathgp::uvector2 id = getObjectCell(newX, newY);
    std::list< std::shared_ptr< Object > >::iterator it;
    unsigned int x, y;
    for (int i = -1; i <= 1; ++i)
    {
        if (i < 0 || i >= m_sizeX)
        {
            continue;
        }
        for (int j = -1; j <= 1; ++j)
        {
            if (j < 0 || j >= m_sizeY)
            {
                continue;
            }
            x = id.x() + i;
            y = id.y() + j;
            it = m_grid[x][y].begin();
            while (it != m_grid[x][y].end())
            {
                if ((*it) != obj)
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
        it = std::find(m_grid[oldId.x()][oldId.y()].begin(), m_grid[oldId.x()][oldId.y()].end(), obj);
        m_grid[oldId.x()][oldId.y()].erase(it);
        m_grid[id.x()][id.y()].push_back(obj);
    }

    return NULL;
}

void ColliderGrid::onObjectCreated(std::shared_ptr<Object> obj)
{
	mathgp::uvector2 id = getObjectCell(obj);
    m_grid[id.x()][id.y()].push_back(obj);
}
void ColliderGrid::onObjectDestroyed(std::shared_ptr<Object> obj)
{
    mathgp::uvector2 id = getObjectCell(obj);
    std::list< std::shared_ptr< Object > >::iterator it = std::find(m_grid[id.x()][id.y()].begin(), m_grid[id.x()][id.y()].end(), obj);
    if (it != m_grid[id.x()][id.y()].end())
    {
        m_grid[id.x()][id.y()].erase(it);
    }
}

mathgp::uvector2 ColliderGrid::getObjectCell(std::shared_ptr<Object> obj)
{
#ifndef _DEBUG
	return mathgp::v((unsigned int)(obj->x() / g_gridSize), (unsigned int)(obj->y() / g_gridSize));
#else
    return cullIdToBounds(mathgp::v((unsigned int)(obj->x() / g_gridSize), (unsigned int)(obj->y() / g_gridSize)));
#endif
}

mathgp::uvector2 ColliderGrid::getObjectCell(float x, float y)
{
#ifndef _DEBUG
    return mathgp::v((unsigned int)(x / g_gridSize), (unsigned int)(y / g_gridSize));
#else
    return cullIdToBounds(mathgp::v((unsigned int)(x / g_gridSize), (unsigned int)(y / g_gridSize)));
#endif
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
bool ColliderGrid::operator()(const std::shared_ptr<Object> obj1, const std::shared_ptr<Object> obj2)
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
        objPos2d.x() = obj->x();
        objPos2d.y() = obj->y();
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

    std::sort(out.begin(), out.end(), ColliderGrid::instance());

    return out;
}

std::vector<std::shared_ptr<Object> > ColliderGrid::collideWithQuadsOnClick(const mathgp::uvector2& screenPos, const mathgp::vector3& worldPoint)
{
    m_currentCollisionRayStart = v(0.0f, 0.0f, 0.0f); //todo: make this player pos;
    const vector3 up = v(0.0f, 0.0f, 1.0f);
    const vector3 left = v(-1.0f, 0.0f, 0.0f);
    std::vector<std::shared_ptr<Object> > out;
    uvector2 id = getObjectCell(worldPoint.x(), worldPoint.y());
    vector3 topLeftWorld;
    vector3 botRightWorld;
    vector2 topLeftScreen;
    vector2 botRightScreen;
    
    const uvector2& screenSize = Application::instance().mainWindow()->clientAreaSize();

    uvector2 halfScreenSize = v(screenSize.x() / 2, screenSize.y() / 2);
   // vector2 ooScreenCoords = v(float((int)screenPos.x() - (int)halfScreenSize.x()) / float(halfScreenSize.x()), float((int)halfScreenSize.y() - (int)screenPos.y()) / float(halfScreenSize.y()));
    
    Camera* cam = Application::instance().currentState()->camera();
    const matrix& projectionView = cam->projectionView();
    //traverse all objects in the cell and all adjacent cells;
    std::list< std::shared_ptr< Object > >::iterator it;
    unsigned int x, y;
    for (int i = -1; i <= 1; ++i)
    {
        if (i < 0 || i >= m_sizeX)
        {
            continue;
        }
        for (int j = -1; j <= 1; ++j)
        {
            if (j < 0 || j >= m_sizeY)
            {
                continue;
            }
            x = id.x() + i;
            y = id.y() + j;
            it = m_grid[x][y].begin();
            while (it != m_grid[x][y].end())
            {
                topLeftWorld = (*it)->position() + (up*(*it)->bb_h()) + (left*((*it)->bb_w() / 2.0f));
                botRightWorld = (*it)->position() + ((-1.0f*left)*((*it)->bb_w() / 2.0f));

                topLeftScreen = transform_coord(topLeftWorld, projectionView).xy();
                topLeftScreen.x() = (topLeftScreen.x()*halfScreenSize.x()) + halfScreenSize.x();
                topLeftScreen.y() = halfScreenSize.y() - (topLeftScreen.y()*halfScreenSize.y());

                botRightScreen = transform_coord(botRightWorld, projectionView).xy();
                botRightScreen.x() = (botRightScreen.x()*halfScreenSize.x()) + halfScreenSize.x();
                botRightScreen.y() = halfScreenSize.y() - (botRightScreen.y()*halfScreenSize.y());

                if (screenPos.x() >= topLeftScreen.x() && screenPos.x() <= botRightScreen.x() &&
                    screenPos.y() >= topLeftScreen.y() && screenPos.y() <= botRightScreen.y())
                {
                    out.push_back(*it);
                }
            }

        }
    }

    std::sort(out.begin(), out.end(), ColliderGrid::instance());

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