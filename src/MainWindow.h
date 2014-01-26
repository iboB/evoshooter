//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// the main window class
#pragma once

class MainWindow
{
public:
    struct CreationParameters
    {
        CreationParameters();

        const char* title;
        mathgp::uvector2 clientAreaSize;
        bool isFullScreen;
    };

    MainWindow(const CreationParameters& params);
    ~MainWindow();

    void setTitle(const char* title);

    const mathgp::uvector2& clientAreaSize() const { return m_creationParameters.clientAreaSize; }

    void swapBuffers();

protected:
    SDL_Window* m_sdlWindow;
    SDL_GLContext m_glContext;

    CreationParameters m_creationParameters;
};
