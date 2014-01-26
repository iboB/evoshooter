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

class Camera;
class Level;
class GUILayer;
class Overlay;

namespace Rocket
{
    namespace Core
    {
        class Element;
    }
}
struct GameHud
{
    Rocket::Core::Element* m_healthDisplay;
    Rocket::Core::Element* m_weaponDisplay;

    void health(int health);
    void weapon(const std::string& weaponName);
};

class ExperimentState : public GameState
{
public:
    ExperimentState();

    void initialize() override;
    void deinitialize() override;

    void handleEvent(const SDL_Event& event) override;
    void update(int dt) override;
    void draw() override;

    Camera* camera() override;
private:
    GUILayer* m_guiLayer;

    Camera* m_camera;
    Level* m_level;

    //Rocket::Core::Element* m_angleDisplay;
    //Rocket::Core::Element* m_distanceDisplay;
    //Rocket::Core::Element* m_fovDisplay;

    GameHud hud;

    float m_camDistance;
    mathgp::vector3 m_camDirection;
    float m_camFov;

    mathgp::vector3 m_camPosition;

    mathgp::vector3 m_moveWeight;

    mathgp::vector3 m_debugStart;
    mathgp::vector3 m_debugEnd;

    Overlay* m_overlay;

};