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
#include "EvoShooter.pch.h"
#include "Level.h"

#include "GLExtensions.h"
#include "Effect.h"
#include "GLSentries.h"
#include "Texture.h"

using namespace mathgp;

Level::Level()
{
    const point3 zero = point3::zero();

    float minx = 0, miny = 0, minz = 0;
    float maxx = 50, maxy = 50, maxz = 20;

    Vertex floor[] =
    {
        { v(minx, miny, minz), vc(0, 0) },
        { v(maxx, miny, minz), vc(0, 1) },
        { v(minx, maxy, minz), vc(1, 0) },
        { v(maxx, maxy, minz), vc(1, 1) },
    };

    m_floor.vb = CreateAndFillBuffer(GL_ARRAY_BUFFER, floor);
    
    m_floor.effect = new Effect;
    m_floor.effect->loadVertexShaderFromFile("level/floor.vert");
    m_floor.effect->loadPixelShaderFromFile("level/level.frag");
    m_floor.effect->link();
    m_floor.pvmParam = m_floor.effect->getParameterByName("pvm");
    m_floor.texParam = m_floor.effect->getParameterByName("colorMap");
    m_floor.tex = new Texture;
    m_floor.tex->loadFromFile("level/floor.png");


    unsigned indices[] =
    {
        0, 1, 2,
        2, 1, 3,
    };
    
    m_indexBuffer = CreateAndFillBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
}

Level::~Level()
{
    destroyFace(m_floor);
    glDeleteBuffers(1, &m_indexBuffer);
}

void Level::draw(const mathgp::matrix& projectionView)
{
    SENTRY(GLEnableAttribSentry, Attr_Pos);
    SENTRY(GLEnableAttribSentry, Attr_TexCoord);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    drawFace(m_floor, projectionView);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Level::drawFace(const Face& face, const mathgp::matrix& projectionView)
{
    face.effect->use();
    face.effect->bindCustomAttribute("inPos", Attr_Pos);
    face.effect->bindCustomAttribute("inTexCoord", Attr_TexCoord);
    face.effect->setParameter(face.pvmParam, projectionView);
    face.effect->setParameter(face.texParam, *face.tex);    

    glBindBuffer(GL_ARRAY_BUFFER, face.vb);
    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSET(0));
    glVertexAttribPointer(Attr_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), OFFSET(sizeof(point3)));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, OFFSET(0));

    //float minx = 0, miny = 0, minz = 0;
    //float maxx = 50, maxy = 50, maxz = 20;

    //Vertex quad[] =
    //{
    //    { v(minx, miny, maxz), vc(0, 0) },
    //    { v(maxx, miny, maxz), vc(0, 1) },        
    //    { v(minx, maxy, maxz), vc(1, 0) },
    //    { v(maxx, maxy, maxz), vc(1, 1) },
    //};

    //unsigned indices[] =
    //{
    //    0, 1, 2,
    //    2, 1, 3,
    //};


    //glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->position);
    //glEnableVertexAttribArray(Attr_Pos);

    //glVertexAttribPointer(Attr_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &quad->texCoord);
    //glEnableVertexAttribArray(Attr_TexCoord);

    //glDrawElements(GL_TRIANGLES, _countof(indices), GL_UNSIGNED_INT, indices);

    //glDisableVertexAttribArray(Attr_Pos);
    //glDisableVertexAttribArray(Attr_TexCoord);
}

void Level::destroyFace(Face& face)
{
    delete face.effect;
    delete face.tex;
    
    glDeleteBuffers(1, &face.vb);    
}