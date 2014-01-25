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

class Camera;

class GameState
{
public:
    GameState();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

    virtual void handleEvent(const SDL_Event& event);
    virtual void update();
    virtual void draw();

    virtual Camera* camera() { return NULL; };

    void pushState(std::shared_ptr<GameState> state);
    void popState();
    void popSelf();

private:
    std::shared_ptr<GameState> m_prevState;
    std::shared_ptr<GameState> m_nextState;
};