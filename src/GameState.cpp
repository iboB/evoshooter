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
#include "GameState.h"

GameState::GameState()
{
}

void GameState::update(int dt)
{
    if (m_nextState)
        m_nextState->update(dt);
}

void GameState::draw()
{
    if (m_nextState)
        m_nextState->draw();
}

void GameState::handleEvent(const SDL_Event& event)
{

}

void GameState::pushState(std::shared_ptr<GameState> state)
{
    m_nextState = state;
    m_nextState->m_prevState = std::shared_ptr<GameState>(this);
    m_nextState->initialize();
}

void GameState::popState()
{
    m_nextState->deinitialize();
    m_nextState.reset();
}

void GameState::popSelf()
{
    assert(m_prevState->m_nextState.get() == this);
    m_prevState->popState();
}