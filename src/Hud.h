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