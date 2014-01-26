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

#define sc static const

sc int Max_Monster_HP = 1000; 
sc int Max_Monster_Stamina = 1000;
sc float Max_Monster_Size = 5;
sc float Max_Monster_Speed = 4;
sc float Aggro_Speed_Factor = 1.5f; // speed when in aggro is multiplied by this
sc float Max_Rest_Time = 4000.f; // max num ms to rest from stamina loss
sc float World_Range_Factor = 0.25f; // max ranges for monster senses as a part of the world size
sc float Max_Aggro_Time = 6000.f; // max time a mon is in aggro
sc float Max_Random_Attack_Wait = 3000.f; // wait max 3 secs between attacks
sc float Max_Loiter_Time = 3000.f; // max time a monster will do nothing

sc float Close_Distance_In_World = 0.3f; // distance to a place in the world considered "there"