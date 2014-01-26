//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//
// the basic texture class
#include "EvoShooter.pch.h"
#include "Texture.h"

using namespace std;

Texture::Texture()
: m_Width(0)
, m_Height(0)
{
    m_Name = "EMPTY_SHITE_OR_LOADED_FROM_DATA_ONLY";
    glGenTextures(1, &m_glHandle);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_glHandle);
}

void Texture::loadFromFile(const char* filename)
{
    SDL_Surface* image = IMG_Load(filename);
    if (!image)
    {
        const char* err = IMG_GetError();
        throw runtime_error(err);
    }

    GLint internalFormat;
    GLenum glFormat;

    auto sdlFormat = image->format->format;

    switch (sdlFormat)
    {
    case SDL_PIXELFORMAT_RGB888:
        internalFormat = GL_RGB8;
        glFormat = GL_RGB;
        break;
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_ABGR8888:
        internalFormat = GL_RGBA8;
        glFormat = GL_RGBA;
        break;
    default:
        internalFormat = GL_RGB8;
        glFormat = GL_RGB;
    //{
    //    cout << "unsupported texture format for " << filename << endl;
    //    assert(false);
    //}
        break;
    }

    loadFromData(internalFormat, image->w, image->h, glFormat, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image);

	m_Name = filename;
}

void Texture::loadFromData(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data)
{
    m_Width = width;
    m_Height = height;

    glBindTexture(GL_TEXTURE_2D, m_glHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    float maxAniso;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
}

void Texture::setParameter(GLenum param, GLint value)
{
    glTexParameteri(GL_TEXTURE_2D, param, value);
}