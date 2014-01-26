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

class Effect;
class Texture;

class Overlay
{
public:
    Overlay();
    ~Overlay();

    void draw();

private:
    Effect* m_effect;
    Texture* m_texture;
    int m_texParam;
    int m_scaleParam;
};