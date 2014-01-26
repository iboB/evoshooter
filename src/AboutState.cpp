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
#include "AboutState.h"
#include "GUILayer.h"
#include "Application.h"

AboutState::AboutState()
    : m_guiLayer(nullptr)
{

}

void AboutState::initialize()
{
    m_guiLayer = new GUILayer("about layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/about.xml");
}

void AboutState::deinitialize()
{
    m_guiLayer->deinitialize();
    delete m_guiLayer;
}

void AboutState::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONDOWN)
    {
        //quit state
        Application::instance().popLastState();
    }
}

void AboutState::update(int dt)
{
    m_guiLayer->update();
}

void AboutState::draw()
{
    m_guiLayer->draw();
}


