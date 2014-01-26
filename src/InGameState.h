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
#include "GameState.h"

class Effect;
class Texture;

class InGameState : public GameState
{
public:
    InGameState(const char* texture, int time);

    void initialize() override;
    void deinitialize() override;

    void handleEvent(const SDL_Event& event) override;
    void update(int dt) override;
    void draw() override;
private:
    Effect* m_effect;
    Texture* m_texture;


    std::string m_textureName;
    int m_time;
};