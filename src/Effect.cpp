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
#include "Effect.h"
#include "Util.h"
#include "Texture.h"

using namespace mathgp;
using namespace std;

size_t Effect::m_numEffects = 0;

Effect::Effect(const string name /*= string()*/)
    : m_name(name)
    , m_id(m_numEffects++)
    , m_glPixelShaderHandle(0)
    , m_glVertexShaderHandle(0)
    , m_boundTextures(0)
{
    m_glProgramHandle = glCreateProgram();
    
    if(!m_glProgramHandle)
    {
        throw(runtime_error("could not create gl program object"));
    }
}

Effect::~Effect()
{
    if(m_glProgramHandle)
        glDeleteProgram(m_glProgramHandle);

    if(m_glPixelShaderHandle)
        glDeleteProgram(m_glPixelShaderHandle);

    if(m_glVertexShaderHandle)
        glDeleteProgram(m_glVertexShaderHandle);
}

void Effect::loadPixelShaderFromFile(const char* filename)
{
    assert(!m_glPixelShaderHandle);
    m_glPixelShaderHandle = loadShaderFromFile(GL_FRAGMENT_SHADER, filename);
}

void Effect::loadVertexShaderFromFile(const char* filename)
{
    assert(!m_glVertexShaderHandle);
    m_glVertexShaderHandle = loadShaderFromFile(GL_VERTEX_SHADER, filename);
}

void Effect::loadPixelShader(const char* data, const char* shaderName /*= ""*/)
{
    assert(data);
    assert(!m_glPixelShaderHandle);
    m_glPixelShaderHandle = loadShader(GL_FRAGMENT_SHADER, data, shaderName);
}

void Effect::loadVertexShader(const char* data, const char* shaderName /*= ""*/)
{
    assert(data);
    assert(!m_glVertexShaderHandle);
    m_glVertexShaderHandle = loadShader(GL_VERTEX_SHADER, data, shaderName);
}

GLuint Effect::loadShaderFromFile(GLenum shaderType, const char* filename)
{
    unique_ptr<const char[]> data(Util::LoadFile(filename));
    if(!data)
    {
        ostringstream sout;
        sout << "could not load " << filename;
        throw runtime_error(sout.str());
    }

    return loadShader(shaderType, data.get(), filename);
}

GLuint Effect::loadShader(GLenum shaderType, const char* shaderData, const char* shaderName)
{
    GLuint shaderHandle = glCreateShader(shaderType);

    if(!shaderHandle)
    {
        ostringstream sout;
        sout << "could not create " << (shaderType == GL_VERTEX_SHADER ? "vs" : "ps") << " object";
        throw runtime_error(sout.str());
    }

    glShaderSource(shaderHandle, 1, &shaderData, nullptr);
    glCompileShader(shaderHandle);

    GLint isCompiled;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);

    GLint infoLen = 0;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLen);

    if(infoLen > 1)
    {
        std::unique_ptr<char[]> infoLog(new char[infoLen]);
        glGetShaderInfoLog(shaderHandle, infoLen, nullptr, infoLog.get());

        if(!isCompiled)
        {
            ostringstream sout;
            sout << "errors compiling " << shaderName << ":" << endl;
            sout << infoLog.get() << endl;
            glDeleteShader(shaderHandle);
            throw runtime_error(sout.str());
        }
        else
        {
            cout << "warnings compiling " << shaderName << ":" << endl;
            cout << infoLog.get() << endl;
        }
    }

    glAttachShader(m_glProgramHandle, shaderHandle);
    return shaderHandle;
}

void Effect::link()
{
    glLinkProgram(m_glProgramHandle);
    
    GLint isLinked;
    glGetProgramiv(m_glProgramHandle, GL_LINK_STATUS, &isLinked);

    if(!isLinked)
    {
        GLint infoLen = 0;
        glGetShaderiv(m_glProgramHandle, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1)
        {
            std::unique_ptr<char[]> infoLog(new char[infoLen]);
            glGetShaderInfoLog(m_glProgramHandle, infoLen, nullptr, infoLog.get());

            ostringstream sout;
            sout << "errors linking " << m_name << ":" << endl;
            sout << infoLog.get() << endl;
            glDeleteProgram(m_glProgramHandle);
            throw runtime_error(sout.str());
        }
        else
        {
            ostringstream sout;
            sout << "unkown errors linking " << m_name;
            glDeleteProgram(m_glProgramHandle);
            throw runtime_error(sout.str());
        }
    }
}

void Effect::use()
{
    glUseProgram(m_glProgramHandle);
    m_boundTextures = 0;
}

void Effect::bindCustomAttribute(const char* name, int index)
{
    glBindAttribLocation(m_glProgramHandle, index, name);
}

GLint Effect::getParameterByName(const char* name) const
{
    return glGetUniformLocation(m_glProgramHandle, name);
}

void Effect::setParameter(GLint handle, float value)
{
    glUniform1f(handle, value);
}

void Effect::setParameter(GLint param, const matrix& matrix)
{
    glUniformMatrix4fv(param, 1, false, matrix.as_array());
}

void Effect::setParameter(GLint param, const vector3& vec)
{
    glUniform3fv(param, 1, vec.as_array());
}

void Effect::setParameter(GLint param, const vector2& vec)
{
    glUniform2fv(param, 1, vec.as_array());
}

void Effect::setParameter(GLint param, const Texture& tex)
{
    glActiveTexture(GL_TEXTURE0 + m_boundTextures);
    glBindTexture(tex.type(), tex.glHandle());
    glUniform1i(param, m_boundTextures);

    ++m_boundTextures;
}
