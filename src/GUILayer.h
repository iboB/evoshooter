//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// gui layer
#pragma once

namespace Rocket
{
    namespace Core
    {
        class Context;
        class ElementDocument;
        class Element;
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

    Rocket::Core::Element* getElementById(const char* id);

private:
    Rocket::Core::Context* m_rocketContext;
    Rocket::Core::ElementDocument* m_rootDocument;

    std::string m_name;
};