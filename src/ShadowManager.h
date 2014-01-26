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
    std::vector<unsigned int> m_indexBuffer;
    TexturePtr m_texture;
};
