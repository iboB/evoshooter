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
#include "ShadowManager.h"
#include "Object.h"
#include "World.h"
#include "ResourceManager.h"
#include "Effect.h"
#include "GLSentries.h"

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
    m_texture = ResourceManager::instance().getTexture("sprites/sprite.png");
}

void ShadowManager::update()
{
    objectsContainer& objects = World::instance().objects();
    Object* obj;
    Quad quad;
    
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

            quad.vertices[0].position = v(0.0f, -1.0f, 0.0f);
            quad.vertices[0].texCoord = v(0.0f, 0.0f);
            quad.vertices[1].position = v(1.0f, 0.0f, 0.0f);
            quad.vertices[1].texCoord = v(0.0f, 1.0f);
            quad.vertices[2].position = v(-1.0f, 0.0f, 0.0f);
            quad.vertices[2].texCoord = v(1.0f, 0.0f);
            quad.vertices[3].position = v(0.0f, 1.0f, 0.0f);
            quad.vertices[3].texCoord = v(1.0f, 1.0f);
            for (int i = 0; i < 4; ++i)
            {
                quad.vertices[i].position *= obj->size();
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
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    SENTRY(GLEnableAttribSentry, Attr_Pos);

    glVertexAttribPointer(Attr_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_quadBuffer.front() + sizeof(vector3));
    SENTRY(GLEnableAttribSentry, Attr_UV);

    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, &m_indexBuffer.front());
}