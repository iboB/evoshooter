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

class Object
{
public:
	Object();
	Object(const mathgp::vector3& pos, float bc);
	virtual ~Object();

	mathgp::vector3 position();
	float boundingCircle();
	float x();
    void  x(float val);
	float y();
    void  y(float val);
	float r();
    bool collidesWith(std::shared_ptr<Object> otherGuy);
    float bb_w();
    float bb_h();

    virtual void update(int dt){}

protected:

	mathgp::vector3 m_pos;
	float m_boundingCircle;
    mathgp::vector2 m_bb; //w,h of the bounding quad
};
