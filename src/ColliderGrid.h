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

#include "ExplicitSingleton.h"
#include "Object.h"
//class Object;

typedef std::vector<std::vector<std::list<std::shared_ptr<Object> > > > gridContainer;

class ColliderGrid : public ExplicitSingleton<ColliderGrid>
{
    DECLARE_EXPLICIT_SINGLETON(ColliderGrid);

public:
	//returns null if move possible, else returns first collider
	std::shared_ptr<Object> requestMoveTo(Object* obj, const mathgp::vector2& pos);
    std::shared_ptr<Object> requestMoveTo(Object* obj, const mathgp::vector3& pos);
    std::shared_ptr<Object> requestMoveTo(Object* obj, float x, float y);
	
	void					onObjectCreated(std::shared_ptr<Object> obj);
	void					onObjectDestroyed(std::shared_ptr<Object> obj);
                            //get all colliding circles in a line
    std::vector<std::shared_ptr<Object> > collideCirclesWith2dRay(mathgp::vector2 start, mathgp::vector2 end);
                            //used to sort outgoing collision data
    bool                    sortCompare(const std::shared_ptr<Object> obj1, const std::shared_ptr<Object> obj2);

    std::vector<std::shared_ptr<Object> > collideWithQuadsOnClick(const mathgp::uvector2& screenPos, const mathgp::vector3& worldPoint);
    //bool                     collideAABBWith3dRay(const std::shared_ptr<Object> obj, const mathgp::vector3& rayStart, const mathgp::vector3& rayDir);

    bool                    isWall(std::shared_ptr<Object> obj) { return (m_dummyObject == obj); }

    void                    initialize();

    std::vector<std::shared_ptr<Object> > collideWithCircle(mathgp::vector2 origin, float r);
private:
    mathgp::uvector2		getObjectCell(Object* obj);
    mathgp::uvector2		getObjectCell(float x, float y);
    mathgp::uvector2&       cullIdToBounds(mathgp::uvector2& id);

    mathgp::vector3         m_currentCollisionRayStart; //used for sorting

	gridContainer m_grid;
	int m_sizeX;
	int m_sizeY;
    std::shared_ptr<Object> m_dummyObject; //dummy obj to simulate wall collisions;
    unsigned int m_dummyObjectId;
public:
    static bool m_collisionMatrix[EObject_Type_Count][EObject_Type_Count];
};
