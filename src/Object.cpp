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
#include "GameplayConstants.h"

const float g_scaleConst = 0.008f;

Object::Object():
m_pos(),
m_boundingCircle(0.0f),
m_bb(mathgp::v(1.0f,1.0f)),
m_renderShadow(false),
m_size(1.0f),
m_id(0)
{
}

Object::Object(const mathgp::vector3& pos, float halfSize) :
m_pos(pos),
m_boundingCircle(halfSize),
m_bb(mathgp::v(halfSize * 2, halfSize * 3)),
m_renderShadow(false),
m_size(halfSize * 2),
m_hitDetectionOffset(mathgp::v(halfSize, halfSize))
{
    //eye aabb hard vals:
    /*
    m_hitDetectionOffset.x() = 0.3;
    m_hitDetectionOffset.y() = 0.1;
    m_bb.x() = 1.0;
    m_bb.y() = 0.5;
    */
    //blob hard vals
    m_hitDetectionOffset.x() = 13 * g_scaleConst;
    m_hitDetectionOffset.y() = 26 * g_scaleConst;
    m_bb.x() = 88 * g_scaleConst;
    m_bb.y() = 120 * g_scaleConst; 

}

Object::~Object()
{
}

mathgp::vector3 Object::position() const
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

float Object::r() const
{
    return m_size*Collision_Circle_Scale_Factor;
}
bool Object::collidesWith(std::shared_ptr<Object> otherGuy)
{
    return (std::sqrt((otherGuy->x() - m_pos.x())*(otherGuy->x() - m_pos.x()) + (otherGuy->y() - m_pos.y())*(otherGuy->y() - m_pos.y())) < m_boundingCircle + otherGuy->r());
}

float Object::bb_w()
{
    return m_bb.y();
}
float Object::bb_h()
{
    return m_bb.x();
}

void Object::OnHit(EAttackDamageType dmgType, int dmg)
{

}