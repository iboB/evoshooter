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
#pragma once

class Camera;

class GameState
{
public:
    GameState();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

    virtual void handleEvent(const SDL_Event& event);
    virtual void update(int dt);
    virtual void draw();

    virtual Camera* camera() { return NULL; };

    void pushState(std::shared_ptr<GameState> state);
    void popState();
    void popSelf();

protected:
    std::shared_ptr<GameState> m_prevState;
    std::shared_ptr<GameState> m_nextState;
};