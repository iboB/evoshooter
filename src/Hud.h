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

class Hud
{
public:
    Hud();
    ~Hud();

    void update();
    void draw();

private:
    struct BasicVertex
    {
        mathgp::point3 position;
    };

    struct BasicQuad
    {
        BasicVertex vertices[4];
    };

    std::vector<BasicQuad> m_basicQuads;

    struct TexVertex
    {
        mathgp::point3 position;
        mathgp::float2 texCoord;
    };

    struct TexQuad
    {
        TexVertex vertices[4];
    };

    std::vector<TexQuad> m_texQuads;

    int m_indices[6];

    Effect* m_colorize;
    Effect* m_texturize;

    Texture* m_charPortrait;
};