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
#include "Camera.h"

#include "Application.h"
#include "MainWindow.h"

#include "Object.h"

using namespace mathgp;

Camera::Camera(const point3& point, const vector3& direction, float distance, float fov)
    : m_point(point)
    , m_view(Mat::identity)
    , m_projection(Mat::identity)
    , m_object(nullptr)
{
    setDirectionAndDistance(direction, distance);
    setFov(fov);
}

void Camera::setFov(float fov)
{
    auto& screenSize = Application::instance().mainWindow()->clientAreaSize();
    float aspect = float(screenSize.x()) / screenSize.y();
    m_projection = matrix::perspective_fov_rh(fov, aspect, 1, 100);
    m_projectionView = m_projection * m_view;
}

void Camera::setDirectionAndDistance(const mathgp::vector3& direction, float distance)
{
    m_position = m_point + normalized(direction) * distance;

    m_direction = direction;

    m_view = matrix::look_at_rh(m_position, m_point, Vec::up);
    m_projectionView = m_projection * m_view;
}

void Camera::moveTo(const point3& point)
{
    auto offset = point - m_point;
    m_position += offset;
    m_point = point;

    m_view = matrix::look_at_rh(m_position, m_point, Vec::up);
    m_projectionView = m_projection * m_view;
}
void Camera::screenToWorldRay(const mathgp::uvector2& screenPos, mathgp::vector3& outStart, mathgp::vector3& outEnd) const
{
    const uvector2& screenSize = Application::instance().mainWindow()->clientAreaSize();
    
    uvector2 halfScreenSize = v(screenSize.x() / 2, screenSize.y() / 2);
    vector2 ooScreenCoords = v(float((int)screenPos.x() - (int)halfScreenSize.x()) / float(halfScreenSize.x()), float((int)halfScreenSize.y() - (int)screenPos.y()) / float(halfScreenSize.y()));

    vector3 start = v(ooScreenCoords.x(), ooScreenCoords.y(), 0.0f);
    vector3 end = v(ooScreenCoords.x(), ooScreenCoords.y(), 1.0f);
    

    outStart = transform_coord(start, inverse(m_projectionView));
    outEnd = transform_coord(end, inverse(m_projectionView));
}
void Camera::screenToWorldPoint(const mathgp::uvector2& screenPos, mathgp::vector3& outPoint, mathgp::vector3& outRayStart, mathgp::vector3& outRayEnd) const
{
    screenToWorldRay(screenPos, outRayStart, outRayEnd);
    vector3 lookAt = normalized(outRayEnd - outRayStart);
    vector3 posOnPlane = v(m_position.x(), m_position.y(), 0.0f);
    vector3 toPlane = normalized(posOnPlane - m_position);
    float distToPlane = std::abs(m_position.z()) / dot(lookAt, toPlane);

    outPoint = lookAt*distToPlane + m_position;
}
void Camera::screenToWorldPoint(const mathgp::uvector2& screenPos, mathgp::vector3& out) const
{
    vector3 rayStart;
    vector3 rayEnd;
    screenToWorldRay(screenPos, rayStart, rayEnd);
    vector3 lookAt = normalized(rayEnd - rayStart);
    vector3 posOnPlane = v(m_position.x(), m_position.y(), 0.0f);
    vector3 toPlane = normalized(posOnPlane - m_position);
    float distToPlane = std::abs(m_position.z()) / dot(lookAt, toPlane);

    out = lookAt*distToPlane + m_position;
}

void Camera::update()
{
    if (!m_object)
    {
        return;
    }

    moveTo(m_object->position());
}

void Camera::followObject(const Object* obj)
{
    m_object = obj;
}