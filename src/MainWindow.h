//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
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
