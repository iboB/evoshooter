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