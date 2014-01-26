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

#include "StaticObject.h"
#include "Sprite.h"
#include "Application.h"
#include "Camera.h"
#include "GameState.h"

StaticObject::StaticObject(SpritePtr sprite, const mathgp::vector3& position, float bc)
: Object(position, bc)
{
    m_Sprite = sprite;
    m_Sprite->setLoop(true);
    m_Sprite->startRendering();
}

StaticObject::~StaticObject()
{
    m_Sprite->stopRendering();
}

void StaticObject::update(int dt)
{
    m_Sprite->update(m_pos, Application::instance().currentState()->camera()->direction());
}
