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

template <typename T>
GLuint CreateAndFillBuffer(GLenum type, const std::vector<T>& data)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(type, buffer);
    glBufferData(type, data_size(data), &data.front(), GL_STATIC_DRAW);
    return buffer;
}

template <typename T, size_t Size>
GLuint CreateAndFillBuffer(GLenum type, const T (&data)[Size])
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(type, buffer);
    glBufferData(type, sizeof(data), data, GL_STATIC_DRAW);
    return buffer;
}

