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

class Level
{
public:
    Level();
    ~Level();

    void draw(const mathgp::matrix& projectionView);

private:
    Effect* m_effect;

    struct Face
    {
        GLuint vb;
        Effect* effect;
        
        Texture* tex;

        int pvmParam;
        int texParam;
    };

    Face m_floor;
    Face m_leftWall;
    Face m_rightWall;
    Face m_back;
    //Face m_front;

    GLuint m_indexBuffer;

    struct Vertex
    {
        mathgp::point3 position;
        mathgp::float2 texCoord;
    };

    enum Attribs
    {
        Attr_Pos, Attr_TexCoord
    };

    void drawFace(const Face& face, const mathgp::matrix& projectionView);
    void destroyFace(Face& face);
};