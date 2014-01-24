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

typedef std::vector<std::vector<std::list<std::shared_ptr<Object> > > > gridContainer;

class ColliderGrid : public ImplicitSingleton<ColliderGrid>
{
public:
	ColliderGrid();
	~ColliderGrid();

	//returns null if move possible, else returns first collider
	std::shared_ptr<Object> requestMoveTo(std::shared_ptr<Object> obj, const mathgp::vector2& pos);
	std::shared_ptr<Object> requestMoveTo(std::shared_ptr<Object> obj, const mathgp::vector3& pos);
	std::shared_ptr<Object> requestMoveTo(std::shared_ptr<Object> obj, float x, float y);
	
	void					onObjectCreated(std::shared_ptr<Object> obj);
	void					onObjectDestroyed(std::shared_ptr<Object> obj);
private:
	mathgp::uvector2		getObjectCell(std::shared_ptr<Object> obj);
    mathgp::uvector2		getObjectCell(float x, float y);
    mathgp::uvector2&       cullIdToBounds(mathgp::uvector2& id);

	gridContainer m_grid;
	int m_sizeX;
	int m_sizeY;
};
