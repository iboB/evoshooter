//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// system interface for libRocket
#pragma once

#include <Rocket/Core/SystemInterface.h>

class GUISystemInterface : public Rocket::Core::SystemInterface
{
public:
    float GetElapsedTime() override;

    bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message) override;
};