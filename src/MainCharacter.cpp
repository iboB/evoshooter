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
#include "EvoShooter.pch.h"

#include "MainCharacter.h"

MainCharacter::MainCharacter(const mathgp::vector3& position)
: Character(position, "player")
{

}

void MainCharacter::update(int dt)
{
    Character::update(dt);
}
