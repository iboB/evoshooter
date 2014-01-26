//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// the main window class definition
#include "EvoShooter.pch.h"
#include "MainWindow.h"

using namespace mathgp;

MainWindow::CreationParameters::CreationParameters()
    : clientAreaSize(mathgp::v(1024u, 768u))
    , title("Main Window")
    , isFullScreen(false)
{
}

MainWindow::MainWindow(const MainWindow::CreationParameters& params)
    : m_creationParameters(params)
{
    const auto& size = params.clientAreaSize;

    int fullScreen = params.isFullScreen ? SDL_WINDOW_FULLSCREEN : 0;

    m_sdlWindow = SDL_CreateWindow(
        params.title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        size.x(), size.y(),
        SDL_WINDOW_OPENGL|fullScreen);

    if(!m_sdlWindow)
    {
        throw std::runtime_error("can't create main window");
    }

    m_glContext = SDL_GL_CreateContext(m_sdlWindow);

    if(!m_sdlWindow)
    {
        throw std::runtime_error("can't create gl context");
    }
}

MainWindow::~MainWindow()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_sdlWindow);
}


void MainWindow::setTitle(const char* title)
{
    SDL_SetWindowTitle(m_sdlWindow, title);
}

void MainWindow::swapBuffers()
{
    SDL_GL_SwapWindow(m_sdlWindow);
}