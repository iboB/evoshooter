//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
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

// monster skillz
sc float Base_Spitter_Range = 5.f;
sc float Spitter_AOE_Radius = 0.3f;
sc float Base_Spitter_Damage = 80.f;
sc float Base_Spitter_Cooldown = 100; //ms
sc float Base_Spitter_Projectile_Speed = 10; // m/s
sc float Base_Spitter_Stamina_Cost = 20; 

sc float Base_Grapple_Range = 5.f;
sc float Grapple_AOE_Radius = 0.3f;
sc float Base_Grapple_Damage = 80.f;
sc float Base_Grapple_Cooldown = 100; //ms
sc float Base_Grapple_Projectile_Speed = 10; // m/s
sc float Base_Grapple_Stamina_Cost = 20;

sc float Base_Thorns_Range = 5.f;
sc float Thorns_AOE_Radius = 0.3f;
sc float Base_Thorns_Damage = 80.f;
sc float Base_Thorns_Cooldown = 100; //ms
sc float Base_Thorns_Projectile_Speed = 10; // m/s
sc float Base_Thorns_Stamina_Cost = 20;

sc float Base_Claws_Range = 5.f;
sc float Claws_AOE_Radius = 0.3f;
sc float Base_Claws_Damage = 80.f;
sc float Base_Claws_Cooldown = 100; //ms
sc float Base_Claws_Projectile_Speed = 10; // m/s
sc float Base_Claws_Stamina_Cost = 20;

sc float Stamina_Loss_Per_Aggro_Second = 10;
sc float Time_To_Lose_1_Stamina_In_Aggro = 1000 / Stamina_Loss_Per_Aggro_Second; //ms

sc float Close_Distance_In_World = 0.3f; // distance to a place in the world considered "there"

sc float Initial_Target_World_Cost = 150;
sc float Target_World_Cost_Rise_Per_Second = 2;

sc float Collision_Circle_Scale_Factor = 0.15f; //size * Collision_Circle_Scale_Factor = collision circle radius
sc float Shadow_Scale_Factor = 0.5f; //scales shadows according to size
sc float Niki_Constant = 0.008f;  //the scale at which obj texture pixes get scaled to world space NOTE: havn't hooked it, dunno where it's used!sc float Niki_Constant = 0.008f;  //the scale at which obj texture pixes get scaled to world space NOTE: havn't hooked it, dunno where it's used!
sc int Player_Starting_Health = 1000;
sc int Knife_Damage_Min = 50;
sc int Knife_Damage_Max = 100;
sc int Pistol_Damage_Min = 10;
sc int Pistol_Damage_Max = 125;