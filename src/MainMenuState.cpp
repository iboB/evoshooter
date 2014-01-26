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
#include "MainMenuState.h"
#include "GUILayer.h"

MainMenuState::MainMenuState()
    : m_guiLayer(nullptr)
{

}

void MainMenuState::initialize()
{
    m_guiLayer = new GUILayer("main menu layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/main.xml");
}

void MainMenuState::deinitialize()
{
    m_guiLayer->deinitialize();
    delete m_guiLayer;
}

void MainMenuState::handleEvent(const SDL_Event& event)
{
    // first try to process it with the GUI
    if (m_guiLayer->processSDLEvent(event))
    {
        return;
    }
}

void MainMenuState::update(int dt)
{
    m_guiLayer->update();
}

void MainMenuState::draw()
{
    m_guiLayer->draw();
}


