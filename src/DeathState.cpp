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
#include "EvoShooter.pch.h"
#include "DeathState.h"
#include "GUILayer.h"
#include "Application.h"
#include "World.h"

DeathState::DeathState()
    : m_guiLayer(nullptr)
{

}

void DeathState::initialize()
{
    m_guiLayer = new GUILayer("death layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/death.xml");
}

void DeathState::deinitialize()
{
    m_guiLayer->deinitialize();
    World::instance().reset();

    delete m_guiLayer;
    
    Application::instance().baseState()->deinitialize();
    Application::instance().baseState()->initialize();
}

void DeathState::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONDOWN)
    {
        //quit state
        Application::instance().popLastState();
    }
}

void DeathState::update(int dt)
{
    m_guiLayer->update();
}

void DeathState::draw()
{
    m_guiLayer->draw();
}


