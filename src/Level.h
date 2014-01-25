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

class Level
{
public:
    Level();
    ~Level();

    void draw();

private:
    Effect* m_effect;

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    size_t m_indexCount;

    struct Vertex
    {
        point3 position;
        float2 texCoord;
    };

    void createQuad(Vertex* target, const mathgp::point3& min, const mathgp::point3 max);
};