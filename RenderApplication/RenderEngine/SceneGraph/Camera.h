#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

class Camera
{
public:
    Camera();
public:
    void Resize(int width, int height);
public:
    void SetPosition(const glm::vec3 pos);
    void SetUp(const glm::vec3 pos);
    void SetTarget(const glm::vec3 pos);
    const glm::vec3& GetPosition() { return m_Position; }
    const glm::vec3& GetUp() { return m_Up; }
    const glm::vec3& GetTarget() { return m_Target; }
    int GetWidth() { return m_nWidth; }
    int GetHeight() { return m_nHeight; }
    entt::entity GetModelId() { return m_ModelId; }
public:
    const glm::mat4& GetView() const;
    const glm::mat4& GetProjection() const;
private:
    void UpdateCamera();
    void SetupCamera();
    void CalculateProjection();
    void CalculateView();
private:
    void CreateModel();
private:
    static void Callback(const glm::mat4& old_value, const glm::mat4& new_value, void* payload);
private:
    glm::vec3   m_Position;
    glm::vec3   m_Target;
    glm::vec3   m_Up;
    glm::mat4   m_Matrix;

    int     m_nWidth;
    int     m_nHeight;

    float m_Fov;
    float m_Near;
    float m_Far;
private:
    glm::mat4   m_View;
    glm::mat4   m_Projection;
private:
    entt::entity    m_ModelId;
};
