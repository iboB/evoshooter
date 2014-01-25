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

class Camera
{
public:
    Camera(const mathgp::point3& point, const mathgp::vector3& direction, float distance, float fov);

    void moveTo(const mathgp::point3& point);

    const mathgp::matrix& projectionView() const { return m_projectionView; }

    void setFov(float fov);
    void setDirectionAndDistance(const mathgp::vector3& direction, float distance);

private:
    mathgp::point3 m_point;
    mathgp::point3 m_position;

    mathgp::matrix m_projection;
    mathgp::matrix m_view;
    mathgp::matrix m_projectionView;
};