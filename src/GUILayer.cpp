//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// gui layer
#include "EvoShooter.pch.h"
#include "GUILayer.h"

#include "Application.h"
#include "GUI.h"
#include "GUIAttribs.h"

#include "GLSentries.h"

#include <Rocket/Core.h>

using namespace mathgp;
using namespace std;

GUILayer::GUILayer(const std::string& name)
    : m_rocketContext(nullptr)
    , m_rootDocument(nullptr)
    , m_name(name)
{
}

void GUILayer::initialize()
{
    const auto& gui = GUI::instance();

    const auto& screenSize = Application::instance().screenSize();

    m_rocketContext = Rocket::Core::CreateContext(m_name.c_str(),
        Rocket::Core::Vector2i(int(screenSize.x()), int(screenSize.y()))
        );
}

void GUILayer::deinitialize()
{
    if (m_rootDocument)
    {
        m_rootDocument->RemoveReference();
        m_rootDocument = nullptr;
    }

    m_rocketContext->RemoveReference();
    m_rocketContext = nullptr;
}

void GUILayer::update()
{
    m_rocketContext->Update();
}

void GUILayer::draw()
{
    SENTRY(GLDisableSentry, GL_DEPTH_TEST);
    SENTRY(GLEnableSentry, GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_rocketContext->Render();

    glDisableVertexAttribArray(Attrib_Position);
    glDisableVertexAttribArray(Attrib_Color);
    glDisableVertexAttribArray(Attrib_TexCoord);
}

void GUILayer::loadRootRml(const std::string& rmlPath)
{
    assert(m_rocketContext);

    m_rootDocument = m_rocketContext->LoadDocument(rmlPath.c_str());

    if (!m_rootDocument)
    {
        cout << "couldn't load rml: " << rmlPath << endl;
        assert(false);
        return;
    }

    m_rootDocument->Show();
}

bool GUILayer::processSDLEvent(const SDL_Event& event)
{
    // process all mouse events (for now)

    if (event.type == SDL_MOUSEMOTION)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        m_rocketContext->ProcessMouseMove(x, y, 0);

        return true;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_rocketContext->ProcessMouseButtonDown(event.button.button-1, 0);
        return true;
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        m_rocketContext->ProcessMouseButtonUp(event.button.button-1, 0);
        return true;
    }

    return false;
}

Rocket::Core::Element* GUILayer::getElementById(const char* id)
{
    return m_rootDocument->GetElementById(id);
}
