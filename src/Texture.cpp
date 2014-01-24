//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// the basic texture class
#include "GDProto.pch.h"
#include "Texture.h"

using namespace std;

Texture::Texture()
{
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
        internalFormat = GL_RGBA8;
        glFormat = GL_RGBA;
        break;
    default:
    {
        cout << "unsupported texture format for " << filename << endl;
        assert(false);
    }
        break;
    }

    loadFromData(internalFormat, image->w, image->h, glFormat, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image);
}

void Texture::loadFromData(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_glHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    float maxAniso;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
}