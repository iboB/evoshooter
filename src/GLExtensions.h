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

