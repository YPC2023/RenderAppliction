#include "Camera.h"
#include "../Core/ConfigSystem.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    m_Position = ConfigSystem::GetCameraPos();
    m_Up = ConfigSystem::GetCameraUp();
    m_Target = ConfigSystem::GetCameraTarget();

    m_Fov = ConfigSystem::GetFov();
    m_Near = ConfigSystem::GetNear();
    m_Far = ConfigSystem::GetFar();

    m_nWidth = ConfigSystem::GetViewportWidth();
    m_nHeight = ConfigSystem::GetViewportHeight();
    SetupCamera();
}

void Camera::Resize(int width, int height)
{
    m_nWidth = width;
    m_nHeight = height;
    SetupCamera();
}

void Camera::SetPosition(const glm::vec3 pos)
{
    m_Position = pos;
    SetupCamera();
}

void Camera::SetUp(const glm::vec3 up)
{
    m_Up = up;
    SetupCamera();
}

void Camera::SetTarget(const glm::vec3 target)
{
    m_Target = target;
    SetupCamera();
}

const glm::mat4& Camera::GetView() const
{
    return m_View;
}

const glm::mat4& Camera::GetProjection() const
{
    return m_Projection;
}

void Camera::SetupCamera()
{
    CalculateView();
    CalculateProjection();
}

void Camera::CalculateProjection()
{
    m_Projection = glm::perspective(glm::radians(m_Fov), ((float)m_nWidth / m_nHeight), m_Near, m_Far);
}

void Camera::CalculateView()
{
    m_View = glm::lookAt(m_Position, m_Target, m_Up);
}