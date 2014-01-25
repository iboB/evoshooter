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
#include "Sprite.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "MonsterCharacter.h"
#include "Camera.h"
#include "GLSentries.h"
#include "Level.h"
#include "GUILayer.h"
#include "Application.h"
#include "Util.h"
#include "Overlay.h"

#include <Rocket/Core/Element.h>

using namespace mathgp;

SpritePtr g_Sprite;
MonsterCharacter* g_Monster;

ExperimentState::ExperimentState()
    : m_camera(nullptr)
    , m_level(nullptr)
    , m_guiLayer(nullptr)
    , m_overlay(nullptr)
{

}

void ExperimentState::initialize()
{
    m_camera = new Camera(m_camPosition = vc(2, 2, 0), m_camDirection = normalized(vc(0, -5, 5)), m_camDistance = 5, m_camFov = mathgp::constants<float>::PI() / 4);
    m_level = new Level;

    m_guiLayer = new GUILayer("gui layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/hud.xml");

    m_angleDisplay = m_guiLayer->getElementById("angle");
    m_fovDisplay = m_guiLayer->getElementById("fov");
    m_distanceDisplay = m_guiLayer->getElementById("dist");

    m_moveWeight = Vec::zero;

    m_effect = new Effect;

    m_effect->loadVertexShaderFromFile("shaders/simple.vert");
    m_effect->loadPixelShaderFromFile("shaders/simple.frag");

    m_effect->link();

    m_texture = new Texture;
    m_texture->loadFromFile("sprites/sprite.png");

    //g_Sprite = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/sprite.png", 2, 4, 8000);
    //g_Sprite = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/jaba_the_slut_die_anim.png", 1, 8, 1000);

    //g_Sprite.reset(new Sprite());

    //g_Sprite->init("sprites/sprite.png", 256, 128, 256, 128, 1, 2, 4000, true);

    //g_Sprite->setScale(0.009f);
    //g_Sprite->setFlipX(true);
    //g_Sprite->startRendering();

    //g_Monster = new MonsterCharacter(mathgp::vc(0.f, 0.f, 0.f), "jaba_the_slut");

    //g_Monster->Move(mathgp::vc(0.f, 0.f, 0.f));

    m_overlay = new Overlay;
}

void ExperimentState::deinitialize()
{
    safe_delete(m_camera);
    safe_delete(m_level);

    m_guiLayer->deinitialize();
    safe_delete(m_guiLayer);

    safe_delete(m_effect);
    safe_delete(m_texture);

    safe_delete(m_overlay);
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
        case SDLK_SPACE:
            //g_Monster->Die();
            break;
        case SDLK_l:
            //g_Monster->GetDamage();
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
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            vector3 start;
            vector3 end;
            m_camera->screenToWorldRay(v((unsigned int)event.button.x, (unsigned int)event.button.y), start, end);
            m_camera->screenToWorldPoint(v((unsigned int)event.button.x, (unsigned int)event.button.y), end);
            m_debugStart = start;
            m_debugEnd = end;
            //std::cout << "end world Pos x:" << end.x() << " y:" << end.y() << std::endl;
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
        //g_Monster->Move(g_Monster->position() + unitsPerSecond * frameTime * normalized(m_moveWeight));
    }

    //g_Sprite->update(vc(0.f, 0.f, 0.0f), m_camDirection);
    //g_Monster->Update(m_camDirection);
}

void ExperimentState::draw()
{
    m_level->draw(m_camera->projectionView());

    RenderManager::instance().Render(m_camera->projectionView());

    struct Vertex
    {
        vector3 pos;
        vector2 uv;
    };

    Vertex quad[] =
    {
        { vc(0, 0, 0), vc(0, 1) },
        { vc(1.8f, 0.0, 0), vc(1, 1) },
        { vc(0, 0.0, 1.8), vc(0, 0) },
        { vc(1.8f, 0.0f, 1.8), vc(1, 0) },
    };

    float camAngle = acos(abs(m_camDirection.y()));
    matrix camAlign = matrix::rotation_x(-camAngle);

    for (auto& vert : quad)
    {
        vert.pos = transform_coord(vert.pos, camAlign);
        vert.pos += m_camPosition;
    }

    unsigned indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };

    const int Attr_Pos = 0;
    const int Attr_UV = 1;

    m_effect->use();
    m_effect->bindCustomAttribute("inPos", Attr_Pos);
    m_effect->bindCustomAttribute("inTexCoord", Attr_UV);

    int pvm = m_effect->getParameterByName("pvm");
    m_effect->setParameter(pvm, m_camera->projectionView());

    int tex = m_effect->getParameterByName("colorMap");
    m_effect->setParameter(tex, *m_texture);

    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->pos);
    glEnableVertexAttribArray(Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->uv);
    glEnableVertexAttribArray(Attr_UV);

    glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, indices);

    srand(10);
    for (int i = 0; i < 20; ++i)
    {
        Vertex quad2[] =
        {
            { vc(0, 0, 0), vc(0, 0) },
            { vc(0.8f, 0, 0), vc(0, 1) },
            { vc(0, 0, 1.28f), vc(1, 0) },
            { vc(0.8f, 0, 1.28f), vc(1, 1) },
        };

        float camAngle = acos(abs(m_camDirection.y()));
        matrix camAlign = matrix::rotation_x(-camAngle);

        vector3 translate = 50 * vc(Util::Rnd01(), Util::Rnd01(), 0);

        for (auto& vert : quad2)
        {
            vert.pos = transform_coord(vert.pos, camAlign);
            vert.pos += translate;
        }

        glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad2->pos);
        glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, indices);
    }
    
    {
        Vertex quad2[] =
        {
            { m_debugStart, vc(0, 0) },
            { m_debugEnd, vc(0, 1) }
          
        };
        int indices[] = { 0, 1 };

        glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad2->pos);
        glDrawElements(GL_LINES, _countof(indices), GL_UNSIGNED_INT, indices);
    }
    
    glDisableVertexAttribArray(Attr_Pos);
    glDisableVertexAttribArray(Attr_UV);

    //g_Sprite->render(m_camera->projectionView());

    m_overlay->draw();

    m_guiLayer->draw();
    
}

Camera* ExperimentState::camera()
{
    return m_camera;
}

