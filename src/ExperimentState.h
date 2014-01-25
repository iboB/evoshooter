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

class Effect;
class Texture;
class Camera;
class Level;
class GUILayer;

namespace Rocket
{
    namespace Core
    {
        class Element;
    }
}


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

    Rocket::Core::Element* m_angleDisplay;
    Rocket::Core::Element* m_distanceDisplay;
    Rocket::Core::Element* m_fovDisplay;

    float m_camDistance;
    mathgp::vector3 m_camDirection;
    float m_camFov;

    mathgp::vector3 m_camPosition;

    mathgp::vector3 m_moveWeight;

    Effect* m_effect;
    Texture* m_texture;

    mathgp::vector3 m_debugStart;
    mathgp::vector3 m_debugEnd;

};