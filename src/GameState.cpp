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
#include "GameState.h"

GameState::GameState()
{
}

void GameState::update()
{
    if (m_nextState)
        m_nextState->update();
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