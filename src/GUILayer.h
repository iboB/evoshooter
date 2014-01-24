//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// gui layer
#pragma once

namespace Rocket
{
    namespace Core
    {
        class Context;
        class ElementDocument;
    }
}

class GUILayer
{
public:
    GUILayer(const std::string& name);

    void initialize();
    void deinitialize();

    void update();
    void draw();

    void loadRootRml(const std::string& rmlPath);

    // return true if event is processed
    bool processSDLEvent(const SDL_Event& event);

private:
    Rocket::Core::Context* m_rocketContext;
    Rocket::Core::ElementDocument* m_rootDocument;

    std::string m_name;
};