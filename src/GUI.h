//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// gui singleton
#pragma once

#include "ExplicitSingleton.h"

class GUIRenderInterface;
class GUISystemInterface;

class GUI : public ExplicitSingleton<GUI>
{
    DECLARE_EXPLICIT_SINGLETON(GUI);
public:

    bool loadFont(const char* fontFilename);

private:
    GUIRenderInterface* m_renderInterface;
    GUISystemInterface* m_systemInterface;
};
