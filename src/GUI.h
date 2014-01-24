//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
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
