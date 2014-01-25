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

class Effect;
class Texture;

class Overlay
{
public:
    Overlay();
    ~Overlay();

    void draw();

private:
    Effect* m_effect;
    Texture* m_texture;
    int m_texParam;
    int m_scaleParam;
};