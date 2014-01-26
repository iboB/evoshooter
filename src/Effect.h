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

#include "NonCopyable.h"

class Texture;

class Effect : NonCopyable
{
public:
    Effect(const std::string name = std::string());
    ~Effect();

    void loadPixelShaderFromFile(const char* filename);
    void loadPixelShader(const char* shaderData, const char* shaderName = "");

    void loadVertexShaderFromFile(const char* filename);
    void loadVertexShader(const char* shaderData, const char* shaderName = "");

    void link();
    void use();

    GLint getParameterByName(const char* name) const;

    void bindCustomAttribute(const char* name, int index);

    void setParameter(GLint handle, float value);
    void setParameter(GLint handle, const mathgp::matrix& matrix);
    void setParameter(GLint handle, const mathgp::vector3& vec);
    void setParameter(GLint handle, const mathgp::vector2& vec);
    void setParameter(GLint handle, const Texture& tex);

    static size_t numEffects() { return m_numEffects; }
    size_t id() const { return m_id; }
private:
    const std::string m_name;

    static size_t m_numEffects;
    const size_t m_id;

    GLuint loadShaderFromFile(GLenum shaderType, const char* filename);
    GLuint loadShader(GLenum shaderType, const char* shaderData, const char* shaderName);
    GLuint m_glProgramHandle;

    GLuint m_glPixelShaderHandle;
    GLuint m_glVertexShaderHandle;

    size_t m_boundTextures;
};
