//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
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

	const std::string& name() const { return m_Name; }

    Uint32& width() { return m_Width; }
    Uint32& height() { return m_Height; }

    void setParameter(GLenum, GLint);
private:
    GLuint m_glHandle;
	std::string m_Name;

    Uint32 m_Width;
    Uint32 m_Height;
};

typedef std::shared_ptr<Texture> TexturePtr;
