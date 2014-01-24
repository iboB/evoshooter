//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// system interface for libRocket
#include "GDProto.pch.h"
#include "GUISystemInterface.h"

#include "Application.h"

using namespace std;

float GUISystemInterface::GetElapsedTime()
{
    return float(Application::instance().currentFrameTime()) / 1000;
}

bool GUISystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
    //if (type <= Rocket::Core::Log::LT_WARNING)
    {
        cout << "libRocket log: " << message.CString() << endl;
    }

    return type <= Rocket::Core::Log::LT_ASSERT;
}