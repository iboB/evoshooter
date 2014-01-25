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

#include "ImplicitSingleton.h"

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class ShadowManager : public ImplicitSingleton<ShadowManager>
{
public:
    ShadowManager();
    ~ShadowManager();

    void update();
    void draw(const mathgp::matrix4& viewProj);
    void initialize();

protected:
    struct Vertex
    {
        mathgp::point3 position;
        mathgp::float2 texCoord;
    };

    struct Quad
    {
        Vertex vertices[4];
    };


    std::vector<Quad> m_quadBuffer;
    std::vector<int> m_indexBuffer;
    TexturePtr m_texture;
};
