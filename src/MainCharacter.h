//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Character.h"

class MainCharacter : public Character
{
public:
    MainCharacter(const mathgp::vector3& position, const std::vector<AttackData>& attacks);

    virtual void update(int dt);
};