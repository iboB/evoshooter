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
#include "ExperimentState.h"
#include "GUILayer.h"
#include "Effect.h"
#include "Texture.h"
#include "Camera.h"
#include "GLSentries.h"
#include "Level.h"
#include "GUILayer.h"
#include "Application.h"

#include <Rocket/Core/Element.h>

using namespace mathgp;

ExperimentState::ExperimentState()
    : m_camera(nullptr)
    , m_level(nullptr)
    , m_guiLayer(nullptr)
{

}

void ExperimentState::initialize()
{
    m_camera = new Camera(m_camPosition = vc(20, 20, 20), m_camDirection = normalized(vc(0, -5, 5)), m_camDistance = 5, m_camFov = mathgp::constants<float>::PI() / 4);
    m_level = new Level;

    m_guiLayer = new GUILayer("gui layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/hud.xml");

    m_angleDisplay = m_guiLayer->getElementById("angle");
    m_fovDisplay = m_guiLayer->getElementById("fov");
    m_distanceDisplay = m_guiLayer->getElementById("dist");

    m_moveWeight = Vec::zero;
}

void ExperimentState::deinitialize()
{
    safe_delete(m_camera);
    safe_delete(m_level);

    m_guiLayer->deinitialize();
    safe_delete(m_guiLayer);
}

void ExperimentState::handleEvent(const SDL_Event& event)
{

    float mod = 0.1f;
    float angle = 0, distance = 0, fov = 0;

    if (event.type == SDL_KEYUP)
    {
        if (event.key.keysym.mod & KMOD_CTRL)
        {
            mod = 0.01f;
        }
        else if (event.key.keysym.mod & KMOD_SHIFT)
        {
            mod = 1;
        }

        switch (event.key.keysym.sym)
        {
        case SDLK_y:
            angle = mod;
            break;
        case SDLK_h:
            angle = -mod;
            break;
        case SDLK_u:
            fov = mod;
            break;
        case SDLK_j:
            fov = -mod;
            break;
        case SDLK_i:
            distance = mod;
            break;
        case SDLK_k:
            distance = -mod;
            break;

        case SDLK_w:
        case SDLK_s:
            m_moveWeight.y() = 0.f;
            break;
        case SDLK_a:
        case SDLK_d:
            m_moveWeight.x() = 0.f;
            break;

        default:
            return;
        }
    }    
    else if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            m_moveWeight.y() = 1.f;
            break;
        case SDLK_s:
            m_moveWeight.y() = -1.f;
            break;
        case SDLK_a:
            m_moveWeight.x() = -1.f;
            break;
        case SDLK_d:
            m_moveWeight.x() = 1.f;
            break;
        }
    }

    m_camera->setFov(m_camFov += fov);
    m_camDistance += distance;
    m_camDirection.z() += angle;
    m_camDirection.normalize();
    m_camera->setDirectionAndDistance(m_camDirection, m_camDistance);
}

void ExperimentState::update()
{
    char text[100];
    sprintf(text, "%.2f", m_camDirection.z());
    m_angleDisplay->SetInnerRML(text);

    sprintf(text, "%.2f", m_camFov);
    m_fovDisplay->SetInnerRML(text);

    sprintf(text, "%.2f", m_camDistance);
    m_distanceDisplay->SetInnerRML(text);

    m_guiLayer->update();


    float unitsPerSecond = 2;
    const unsigned char* keyStates = SDL_GetKeyboardState(nullptr);
    if (keyStates[SDL_SCANCODE_LSHIFT])
        unitsPerSecond *= 5;

    float frameTime = float(Application::instance().timeSinceLastFrame())/1000;

    if (m_moveWeight.length_sq() > 0.5)
    {
        m_camPosition += unitsPerSecond * frameTime * normalized(m_moveWeight);
        m_camera->moveTo(m_camPosition);
    }
}

void ExperimentState::draw()
{
    m_level->draw(m_camera->projectionView());
    m_guiLayer->draw();
}


