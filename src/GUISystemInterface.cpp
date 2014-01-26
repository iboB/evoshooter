//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// system interface for libRocket
#include "EvoShooter.pch.h"
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