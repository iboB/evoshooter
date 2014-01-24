//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// gui singleton
#include "GDProto.pch.h"
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