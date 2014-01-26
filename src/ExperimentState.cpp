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
#include "ExperimentState.h"
#include "GUILayer.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "MonsterCharacter.h"
#include "MainCharacter.h"
#include "Camera.h"
#include "Bullet.h"
#include "StaticObject.h"
#include "GLSentries.h"
#include "Level.h"
#include "GUILayer.h"
#include "SpawnManager.h"
#include "Application.h"
#include "Util.h"
#include "Overlay.h"
#include "SoundManager.h"
#include "World.h"
#include "ColliderGrid.h"
#include "SoundManager.h"
#include "AboutState.h"
#include "ShadowManager.h"
#include <iostream>

#include "Hud.h"

#include <Rocket/Core/Element.h>

using namespace mathgp;

//SpritePtr g_Sprite;
MonsterCharacter* g_Monster;

bool m_isShooting;
unsigned m_lastShotTime;

void GameHud::health(int health, int maxHealth)
{
    char text[50];
    sprintf(text, "%i/%i", health, maxHealth);
    m_healthDisplay->SetInnerRML(text);
}

void GameHud::weapon(const std::string& weaponName)
{
    m_weaponDisplay->SetInnerRML(weaponName.c_str());
}

ExperimentState::ExperimentState()
    : m_camera(nullptr)
    , m_level(nullptr)
    , m_guiLayer(nullptr)
    , m_overlay(nullptr)
    , m_hud(nullptr)
{

}

void ExperimentState::initialize()
{
    World::createInstance();
    ColliderGrid::createInstance();

    m_camera = new Camera(m_camPosition = vc(2, 2, 0), m_camDirection = normalized(vc(0, -5, 3.5)), m_camDistance = 6, m_camFov = mathgp::constants<float>::PI() / 4);
    m_level = new Level;

    m_guiLayer = new GUILayer("gui layer");
    m_guiLayer->initialize();
    m_guiLayer->loadRootRml("gui/game_hud.xml");

    //m_angleDisplay = m_guiLayer->getElementById("angle");
    //m_fovDisplay = m_guiLayer->getElementById("fov");
    //m_distanceDisplay = m_guiLayer->getElementById("dist");
    hud.m_healthDisplay = m_guiLayer->getElementById("health");
    hud.m_weaponDisplay = m_guiLayer->getElementById("weapon");

    m_moveWeight = Vec::zero;

    //g_Sprite = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/sprite.png", 2, 4, 8000);
    //g_Sprite = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/sprite.png", 2, 4, 8000);

    //g_Sprite.reset(new Sprite());

    //g_Sprite->init("sprites/sprite.png", 256, 128, 256, 128, 1, 2, 4000, true);

    //g_Sprite->setScale(0.02f);
    //g_Sprite->setFlipX(true);
    //g_Sprite->startRendering();

    //SoundManager::instance().playTrack(0, true);

    m_overlay = new Overlay;

    AttacksData attacks;
    AttackData a1 = { "sprites/attacks/attack_anim_01.png", "sprites/attacks/attack_anim_idle_01.png", Vec::zero, 0.005f, false };
    AttackData a2 = { "sprites/attacks/attack_anim_02.png", "sprites/attacks/attack_anim_idle_02.png", Vec::zero, 0.005f, false };
    AttackData a3 = { "sprites/attacks/attack_anim_03.png", "sprites/attacks/attack_anim_idle_03.png", mathgp::vc(0.f, 0.25f, 0.f), 0.005f, false };
    attacks.push_back(a1);
    attacks.push_back(a2);
    attacks.push_back(a3);

    AttacksData attacks2;
    AttackData a11 = { "sprites/attacks/attack_anim_01.png", "sprites/attacks/attack_anim_idle_01.png", Vec::zero, 0.005f, false };
    AttackData a21 = { "sprites/attacks/attack_anim_02.png", "sprites/attacks/attack_anim_idle_02.png", Vec::zero, 0.005f, false };
    //AttackData a31 = { "sprites/attacks/attack_anim_00.png", "sprites/attacks/attack_anim_idle_00.png", Vec::zero, 0.008f, true };
    attacks2.push_back(a11);
    attacks2.push_back(a21);
    //attacks2.push_back(a31);

    //id = World::instance().spawnMonster(2.f, 3.f, 0.5f, "eye", attacks);
    
    World::instance().spawnPlayer(11.f, 11.f, 0.5f, attacks2);

    m_camera->followObject(World::instance().mainCharacter());
    ShadowManager::instance().initialize();
    ColliderGrid::instance().initialize();

    m_hud = new Hud;
}

void ExperimentState::deinitialize()
{
    safe_delete(m_hud);
    safe_delete(m_camera);
    safe_delete(m_level);

    m_guiLayer->deinitialize();
    safe_delete(m_guiLayer);

    hud.m_healthDisplay = NULL;
    hud.m_weaponDisplay = NULL;

    safe_delete(m_overlay);

    ColliderGrid::destroyInstance();
    World::destroyInstance();
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
        
        case SDLK_w:
        case SDLK_s:
            m_moveWeight.y() = 0.f;
            break;
        case SDLK_a:
        case SDLK_d:
            m_moveWeight.x() = 0.f;
            break;       
        case SDLK_1:
            {
                World::instance().mainCharacter()->previousWeapon();
            }
            break;
        case SDLK_2:
            {
                World::instance().mainCharacter()->nextWeapon();
            }
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
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            m_isShooting = true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            m_isShooting = false;
        }
    }

    m_camera->setFov(m_camFov += fov);
    m_camDistance += distance;
    m_camDirection.z() += angle;
    m_camDirection.normalize();
    m_camera->setDirectionAndDistance(m_camDirection, m_camDistance);    
}

void ExperimentState::weaponDisplay(const std::string& str)
{
    hud.weapon(str);
}

void ExperimentState::update(int dt)
{
    /*
    char text[100];
    
    sprintf(text, "%.2f", m_camDirection.z());
    m_angleDisplay->SetInnerRML(text);

    sprintf(text, "%.2f", m_camFov);
    m_fovDisplay->SetInnerRML(text);

    sprintf(text, "%.2f", m_camDistance);
    m_distanceDisplay->SetInnerRML(text);
    */
    hud.health(World::instance().mainCharacter()->hp(), World::instance().mainCharacter()->maxHp());
    //hud.weapon("Knife");

    m_guiLayer->update();


    float unitsPerSecond = 2;
    const unsigned char* keyStates = SDL_GetKeyboardState(nullptr);
    if (keyStates[SDL_SCANCODE_LSHIFT])
        unitsPerSecond *= 5;

    float frameTime = float(Application::instance().timeSinceLastFrame())/1000;

    if (m_moveWeight.length_sq() > 0.5)
    {
        m_camPosition += unitsPerSecond * frameTime * normalized(m_moveWeight);
        
        World::instance().mainCharacter()->Move(World::instance().mainCharacter()->position() + unitsPerSecond * frameTime * normalized(m_moveWeight));
    }

    SpawnManager::instance().update(dt);
    World::instance().update(dt);
    //g_Sprite->update(vc(2.f, 4.f, 0.0f), m_camDirection);

    m_camera->update();
	ShadowManager::instance().update();

    m_hud->update();

    if (m_isShooting)
    {
        unsigned now = Application::instance().currentFrameTime();

        if (now - m_lastShotTime > 100)
        {

            int x, y;
            SDL_GetMouseState(&x, &y);
            //vector3 start;
            vector3 worldPoint;
            //m_camera->screenToWorldRay(v((unsigned int)event.button.x, (unsigned int)event.button.y), start, end);
            m_camera->screenToWorldPoint(v((unsigned int)x, (unsigned int)y), worldPoint);
            //m_debugStart = start;
            //m_debugEnd = end;
            std::vector< std::shared_ptr<Object>> test = ColliderGrid::instance().collideWithQuadsOnClick(v((unsigned int)x, (unsigned int)y), worldPoint);
            Object * obj = NULL;
            if (test.size() > 0)
                obj = test[0].get();

            World::instance().mainCharacter()->useWeapon(worldPoint, obj);
            //std::cout << "end world Pos x:" << end.x() << " y:" << end.y() << std::endl;

            m_lastShotTime = now;
        }
    }
}

void ExperimentState::draw()
{
    m_level->draw(m_camera->projectionView());

    ShadowManager::instance().draw(m_camera->projectionView());

    RenderManager::instance().Render(m_camera->projectionView());

    //g_Sprite->render(m_camera->projectionView());
    //g_Sprite->render(m_camera->projectionView());

    m_overlay->draw();

    m_hud->draw();

    m_guiLayer->draw();
    
}

Camera* ExperimentState::camera()
{
    return m_camera;
}

