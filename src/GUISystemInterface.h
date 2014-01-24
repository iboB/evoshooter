//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
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