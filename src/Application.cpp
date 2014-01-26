//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// main application class definition
#include "EvoShooter.pch.h"
#include "Application.h"
#include "MainWindow.h"

#include "VersionInfo.h"

#include "GUI.h"

#include "InGameState.h"
#include "ExperimentState.h"
#include "AboutState.h"
#include "MainMenuState.h"

using namespace std;
using namespace mathgp;

Application::Application()
    : m_mainWindow(nullptr)
    , m_mainScene(nullptr)
    , m_isRunning(false)
    , m_currentFrameTime(0)
    , m_desiredFrameTime(10) // work with 100 fps
    // fps stuff
    , m_frameCount(0)
    , m_lastFrameEnd(0)
    , m_lastFPSStatusUpdateTime(0)
    , m_lastFPSStatusUpdateFrameCount(0)
    , m_timeSinceLastFrame(0)
    // debug stuff
    , m_isWireframe(false)
    // states
    , m_baseState(nullptr)
{
}

Application::~Application()
{
}

#include "ResourceManager.h"
#include "Sprite.h"
#include "Texture.h"

void Application::run()
{
    initialize();

    /*TexturePtr texture = ResourceManager::instance().getTexture("Cracked-Facebook-Logo.png");
    TexturePtr texture2 = ResourceManager::instance().getTexture("Cracked-Facebook-Logo.png");
    TexturePtr texture3 = ResourceManager::instance().getTexture("Cracked-Facebook-Logo.png");

    ResourceManager::instance().releaseTexture(texture);
    ResourceManager::instance().releaseTexture(texture2);

    {
        Sprite s1;
        s1.init("Cracked-Facebook-Logo.png", 32, 64, 32, 64, 4, 8, 2000, true);
        s1.startAnimation();
        s1.render();
        s1.render();

        Sprite s2;
        s2.init("Cracked-Facebook-Logo.png", 0, 0, 0, 0, 0, 0, 0, true);
        s2.startAnimation();
        s2.render();
    }


    ResourceManager::instance().releaseTexture(texture3);
*/
    m_currentFrameTime = SDL_GetTicks() - 1;

    m_isRunning = true;
    while(m_isRunning)
    {
        unsigned now = SDL_GetTicks();
        m_timeSinceLastFrame = now - m_currentFrameTime;
        m_currentFrameTime = now;

        handleInput();

        //m_baseState->update();
        currentState()->update(m_timeSinceLastFrame);

        drawFrame();
        updateFPSData();

        // force framerate
        unsigned frameLength = SDL_GetTicks() - m_currentFrameTime;
        if(frameLength < m_desiredFrameTime)
        {
            SDL_Delay(m_desiredFrameTime - frameLength);
        }
    }

    deinitialize();
}

void Application::initialize()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw runtime_error("couldn't init sdl");
    }

    MainWindow::CreationParameters mwc;
    mwc.clientAreaSize = v(1024u, 600u);
    mwc.isFullScreen = false;
    mwc.title = APP_NAME;
    m_mainWindow = new MainWindow(mwc);

#if defined(_WIN32)
    if(glewInit() != GLEW_OK)
    {
        throw runtime_error("couldn't init glew");
    }
#endif
 
    //////////////////////////////////////
    // graphhics

    // global program features
    glEnable(GL_DEPTH_TEST); // z buffer
    glEnable(GL_CULL_FACE); // cull back (CW) faces

    glClearColor(0, 0, 0.6f, 0); // backbuffer clear color
    glClearDepth(1); // z buffer clear value

    //////////////////////////////////////
    // gui
    GUI::createInstance();
    GUI::instance().loadFont("gui/fonts/atari.ttf");

    //////////////////////////////////////
    // state
    //m_baseState = new InGameState;
    m_baseState = new ExperimentState;
    m_baseState->initialize();
    //m_baseState = new AboutState;
    //m_baseState->initialize();
    m_stateStack.push_back(m_baseState);
}

void Application::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //m_baseState->draw();
    if (currentState() != m_baseState)
    {
        m_baseState->draw();
    }
    currentState()->draw();

    m_mainWindow->swapBuffers();
}

void Application::deinitialize()
{
    m_baseState->deinitialize();
    delete m_baseState;
    GUI::destroyInstance();

    safe_delete(m_mainWindow);
    SDL_Quit();
}

void Application::updateFPSData()
{
    ++m_frameCount;
    
    const unsigned now = SDL_GetTicks();
    const unsigned frameTime = now - m_lastFrameEnd;
    const unsigned fpsStatusUpdateTimeDelta = now - m_lastFPSStatusUpdateTime;

    if(fpsStatusUpdateTimeDelta > 1000) // update once per second
    {
        const unsigned framesDelta = m_frameCount - m_lastFPSStatusUpdateFrameCount;
        const unsigned meanFrameTime = fpsStatusUpdateTimeDelta/framesDelta;
        const unsigned fps = (framesDelta*1000)/fpsStatusUpdateTimeDelta;

        string windowTitle;
        ostringstream sout;
        sout << APP_NAME << " " << APP_VERSION_STRING << " | mean frame time: " << meanFrameTime << " ms | fps: " << fps;

        m_mainWindow->setTitle(sout.str().c_str());

        m_lastFPSStatusUpdateTime = now;
        m_lastFPSStatusUpdateFrameCount = m_frameCount;
    }
}

void Application::handleInput()
{
    SDL_Event event;
    bool handledHere = false;
    while(SDL_PollEvent(&event))
    {
        //m_baseState->handleEvent(event);
        currentState()->handleEvent(event);

        if(event.type == SDL_QUIT)
        {
            m_isRunning = false;
            handledHere = true;
        }
        else if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_isRunning = false;
                handledHere = true;
                break;
            case SDLK_p:
                m_isWireframe = !m_isWireframe;
                glPolygonMode(GL_FRONT_AND_BACK, m_isWireframe ? GL_LINE : GL_FILL);
                handledHere = true;
                break;
            }
        }

        if(!handledHere)
        {
            // handle in some input controller
        }
    }
}

const uvector2& Application::screenSize() const
{
    return m_mainWindow->clientAreaSize();
}

void Application::popLastState()
{
    if (m_stateStack.size() > 0)
    {
        unsigned int lastStateIdx = m_stateStack.size() - 1;
        m_stateStack[lastStateIdx]->deinitialize();
        delete m_stateStack[lastStateIdx];
        m_stateStack.pop_back();
    }
}
void Application::pushState(GameState* state)
{
    state->initialize();
    m_stateStack.push_back(state);
}