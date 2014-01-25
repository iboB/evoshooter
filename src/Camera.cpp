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
#include "EvoShooter.pch.h"
#include "Camera.h"

#include "Application.h"
#include "MainWindow.h"

using namespace mathgp;

Camera::Camera(const point3& point, const vector3& direction, float distance, float fov)
    : m_point(point)
    , m_view(Mat::identity)
    , m_projection(Mat::identity)
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