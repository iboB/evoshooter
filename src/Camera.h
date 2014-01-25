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

class Object;

class Camera
{
public:
    Camera(const mathgp::point3& point, const mathgp::vector3& direction, float distance, float fov);

    const mathgp::matrix& projectionView() const { return m_projectionView; }
    const mathgp::vector3& direction() const { return m_direction; }

    void setFov(float fov);
    void setDirectionAndDistance(const mathgp::vector3& direction, float distance);

    void screenToWorldRay(const mathgp::uvector2& screenPos, mathgp::vector3& outStart, mathgp::vector3& outEnd) const;
    //gives a point on the world plane
    void screenToWorldPoint(const mathgp::uvector2& screenPos, mathgp::vector3& out) const;
    void screenToWorldPoint(const mathgp::uvector2& screenPos, mathgp::vector3& outPoint, mathgp::vector3& outRayStart, mathgp::vector3& outRayEnd) const;

    void update();

    void followObject(const Object* obj);
private:
    void moveTo(const mathgp::point3& point);

    mathgp::point3 m_point;
    mathgp::point3 m_position;

    mathgp::vector3 m_direction;

    mathgp::matrix m_projection;
    mathgp::matrix m_view;
    mathgp::matrix m_projectionView;

    const Object* m_object;
};