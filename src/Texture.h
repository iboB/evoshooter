//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// the basic texture class
#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void loadFromFile(const char* filename);
    void loadFromData(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data);

    GLenum type() const { return GL_TEXTURE_2D; }
    GLuint glHandle() const { return m_glHandle; }
private:
    GLuint m_glHandle;
};