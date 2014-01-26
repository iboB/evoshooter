//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// gui singleton
#include "EvoShooter.pch.h"
#include "GUI.h"

#include "GUISystemInterface.h"
#include "GUIRenderInterface.h"

#include <Rocket/Core/Core.h>

using namespace std;

GUI::GUI()
{
    m_systemInterface = new GUISystemInterface;
    Rocket::Core::SetSystemInterface(m_systemInterface);

    m_renderInterface = new GUIRenderInterface;
    Rocket::Core::SetRenderInterface(m_renderInterface);

    if (!Rocket::Core::Initialise())
    {
        throw runtime_error("couldn't initialize libRocket");
    }
}

GUI::~GUI()
{
    Rocket::Core::Shutdown();
}

bool GUI::loadFont(const char* fontFilename)
{
    return Rocket::Core::FontDatabase::LoadFontFace(fontFilename);
}