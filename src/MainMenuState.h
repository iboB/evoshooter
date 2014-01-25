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

#include "GameState.h"

class GUILayer;

class MainMenuState : public GameState
{
public:
    MainMenuState();

    void initialize() override;
    void deinitialize() override;

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void draw() override;

private:
    GUILayer* m_guiLayer;
};