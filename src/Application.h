//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// main application class declaration
#pragma once

#include "ImplicitSingleton.h"

class MainWindow;
class Scene;
class IObjectController;
class Renderer;
class GameState;


class Application : public ImplicitSingleton<Application>
{
public:
    Application();
    ~Application();

    void run();

    const mathgp::uvector2& screenSize() const;

    MainWindow* mainWindow() const { return m_mainWindow; }
    GameState* currentState() const { return m_stateStack[m_stateStack.size()-1]; }
    GameState* baseState() const { return m_baseState;  }
    void popLastState();
    void pushState(GameState* state);

    void experiMENTAL();
private:
    void initialize();
    void deinitialize();

    MainWindow* m_mainWindow;

    bool m_isRunning;

    Scene* m_mainScene;

    //////////////////////////////////////
    // input
    void handleInput();

    //////////////////////////////////////
    // rendering
    void drawFrame();

    //////////////////////////////////////
    // time
public:
    unsigned currentFrameTime() const { return m_currentFrameTime; }
    unsigned timeSinceLastFrame() const { return m_timeSinceLastFrame;  }

private:
    unsigned m_currentFrameTime; // start of current frame (ms)
    unsigned m_timeSinceLastFrame;

    //////////////////////////////////////
    // fps stuff
    unsigned  m_desiredFrameTime; // the application will try to work at this time per frame

    void updateFPSData();

    unsigned m_frameCount; // number of frames since the app has started
    unsigned m_lastFrameEnd; // end time of the last frame
    unsigned m_lastFPSStatusUpdateTime; // last time the fps status was updated
    unsigned m_lastFPSStatusUpdateFrameCount; // frame count of the last time the stasus was updated

    //////////////////////////////////////
    // debugging and so forth
    bool m_isWireframe;

    //////////////////////////////////////
    // states
    GameState* m_baseState;
    std::vector<GameState*> m_stateStack;
};
