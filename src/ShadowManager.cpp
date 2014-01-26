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
#include "EvoShooter.pch.h"
#include "ShadowManager.h"
#include "Object.h"
#include "World.h"
#include "ResourceManager.h"
#include "Effect.h"
#include "GLSentries.h"
#include "GameplayConstants.h"

using namespace mathgp;

ShadowManager::ShadowManager()
{
}

ShadowManager::~ShadowManager()
{
    if (m_texture)
    {
        ResourceManager::instance().releaseTexture(m_texture);
    }
}

void ShadowManager::initialize()
{
    m_texture = ResourceManager::instance().getTexture("level/shadows_gradient.png");
}

void ShadowManager::update()
{
    objectsContainer& objects = World::instance().objects();
    Object* obj;
    Quad quad;
    m_quadBuffer.clear();
    m_indexBuffer.clear();
    unsigned count = 0;
    unsigned startIndex = 0;
    const unsigned indexStep = 4;

    objectsContainer::iterator it = objects.begin();
    
    while (it != objects.end())
    {
        obj = it->second.get();
        if (obj->shouldRenderShadow())
        {
            //fill index buffer for obj
            startIndex = count * indexStep;
            m_indexBuffer.push_back(startIndex);
            m_indexBuffer.push_back(startIndex + 1);
            m_indexBuffer.push_back(startIndex + 2);
            m_indexBuffer.push_back(startIndex + 2);
            m_indexBuffer.push_back(startIndex + 1);
            m_indexBuffer.push_back(startIndex + 3);
            ++count;
            /* { v(maxx, maxy, minz), vc(0, 1) },
        { v(maxx, miny, minz), vc(1, 1) },
        { v(maxx, maxy, maxz), vc(0, 0) },
        { v(maxx, miny, maxz), vc(1, 0) },
    };*/
            quad.vertices[0].position = v(0.0f, 0.0f, 0.0f);
            quad.vertices[0].texCoord = v(0.0f, 1.0f);
            quad.vertices[1].position = v(1.0f, 0.0f, 0.0f);
            quad.vertices[1].texCoord = v(1.0f, 1.0f);
            quad.vertices[2].position = v(0.0f, 1.0f, 0.0f);
            quad.vertices[2].texCoord = v(0.0f, 0.0f);
            quad.vertices[3].position = v(1.0f, 1.0f, 0.0f);
            quad.vertices[3].texCoord = v(1.0f, 0.0f);
            for (int i = 0; i < 4; ++i)
            {
                quad.vertices[i].position *= (obj->size()*Shadow_Scale_Factor);
                //quad.vertices[i].position = (quad.vertices[i].position - obj->position()) + obj->position();
                quad.vertices[i].position += obj->position();
                quad.vertices[i].position.z() = 0.001f;
            }

            m_quadBuffer.push_back(quad);
        }
        ++it;
    }
}

void ShadowManager::draw(const mathgp::matrix4& viewProj)
{    
    SENTRY(GLEnableSentry, GL_BLEND);
    SENTRY(GLDisableSentry, GL_DEPTH_TEST);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    const int Attr_Pos = 0;
    const int Attr_UV = 1;

    Effect* simpleEffect = ResourceManager::instance().getSimpleEffect();

    simpleEffect->use();
    simpleEffect->bindCustomAttribute("inPos", Attr_Pos);
    simpleEffect->bindCustomAttribute("inTexCoord", Attr_UV);

    int pvm = simpleEffect->getParameterByName("pvm");
    simpleEffect->setParameter(pvm, viewProj);

    int tex = simpleEffect->getParameterByName("colorMap");
    simpleEffect->setParameter(tex, *m_texture);

    glVertexAttribPointer(Attr_Pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_quadBuffer.front());
    glEnableVertexAttribArray(Attr_Pos);
    SENTRY(GLEnableAttribSentry, Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<char*>(&m_quadBuffer.front()) + sizeof(vector3));
    SENTRY(GLEnableAttribSentry, Attr_UV);

    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, &m_indexBuffer.front());

}