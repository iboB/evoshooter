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
#include "Object.h"
#include <math.h> 

Object::Object():
m_pos(),
m_boundingCircle(0.0f)
{
}

Object::Object(const mathgp::vector3& pos, float bc):
m_pos(pos),
m_boundingCircle(bc)
{
}

Object::~Object()
{
}

mathgp::vector3 Object::position()
{
	return m_pos;
}
float Object::boundingCircle()
{
	return m_boundingCircle;
}

float Object::x()
{
	return m_pos.x();
}

void Object::x(float val)
{
    m_pos.x() = val;
}

float Object::y()
{
	return m_pos.y();
}

void Object::y(float val)
{
    m_pos.y() = val;
}

float Object::r()
{
	return m_boundingCircle;
}
bool Object::collidesWith(std::shared_ptr<Object> otherGuy)
{
    return (std::sqrt((otherGuy->x() - m_pos.x())*(otherGuy->x() - m_pos.x()) + (otherGuy->y() - m_pos.y())*(otherGuy->y() - m_pos.y())) < m_boundingCircle + otherGuy->r());
}