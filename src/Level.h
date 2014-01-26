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