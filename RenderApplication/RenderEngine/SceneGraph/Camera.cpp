#include "Camera.h"
#include <CUtils.h>
#include "SceneGraph.h"
#include "../Core/ConfigSystem.h"
#include "../Model/CModelLoader.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    static int index = 1;
    m_ModelId = entt::null;

    m_Position = ConfigSystem::GetCameraPos();
    m_Up = ConfigSystem::GetCameraUp();
    m_Target = ConfigSystem::GetCameraTarget();

    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(index++ * 130.0f), m_Up);
    m_Position = glm::vec3(rollMatrix * glm::vec4(m_Position, 0.0f));

    m_Fov = ConfigSystem::GetFov();
    m_Near = ConfigSystem::GetNear();
    m_Far = ConfigSystem::GetFar();

    m_nWidth = ConfigSystem::GetViewportWidth();
    m_nHeight = ConfigSystem::GetViewportHeight();
    SetupCamera();
}

Camera::~Camera()
{
    std::set<entt::entity> setNode = SceneGraph::GetInstance().GetModelTransformComponents(m_ModelId);
    for (auto entity : setNode) {
        SceneGraph::GetInstance().RemoveNode(entity);
    }
}

void Camera::Resize(int width, int height)
{
    m_nWidth = width;
    m_nHeight = height;
    UpdateCamera();
}

void Camera::SetPosition(const glm::vec3 pos)
{
    m_Position = pos;
    UpdateCamera();
}

void Camera::SetUp(const glm::vec3 up)
{
    m_Up = up;
    UpdateCamera();
}

void Camera::SetTarget(const glm::vec3 target)
{
    m_Target = target;
    UpdateCamera();
}

const glm::mat4& Camera::GetView() const
{
    return m_View;
}

const glm::mat4& Camera::GetProjection() const
{
    return m_Projection;
}

void Camera::UpdateCamera()
{
    UpdateView();
    UpdateProjection();
}

void Camera::SetupCamera()
{
    CreateModel();
    UpdateCamera();
}

void Camera::UpdateProjection()
{
    m_Projection = glm::perspective(glm::radians(m_Fov), ((float)m_nWidth / m_nHeight), m_Near, m_Far);
}

void Camera::UpdateView()
{
    glm::mat4 matrix = GetTransformData();

    glm::vec3 Position = glm::vec3(matrix * glm::vec4(m_position, 1.0f));
    glm::vec3 Target = glm::vec3(matrix * glm::vec4(m_target, 1.0f));
    //glm::vec3 Up = glm::vec3(m_matrix * glm::vec4(m_up, 1.0f));
    m_View = glm::lookAt(Position, Target, m_Up);
}

void Camera::SetTransformData(const glm::mat4& matrix)
{
    // 平移项链
    glm::vec3 translation = glm::vec3(matrix[3]);
    // 缩放向量
    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(matrix[0]));
    scale.y = glm::length(glm::vec3(matrix[1]));
    scale.z = glm::length(glm::vec3(matrix[2]));
    // 先移除缩放
    glm::mat3 rotationMatrix;
    rotationMatrix[0] = glm::vec3(matrix[0]) / scale.x;
    rotationMatrix[1] = glm::vec3(matrix[1]) / scale.y;
    rotationMatrix[2] = glm::vec3(matrix[2]) / scale.z;
    glm::quat rotationQuat = glm::quat_cast(rotationMatrix);

    auto& Transform = SceneGraph::GetInstance().GetCmpntTransformData(m_ModelId);
    Transform.translation = translation;
    Transform.rotation = rotationMatrix;
    Transform.scale = scale;
}

glm::mat4 Camera::GetTransformData()
{
    auto& Transform = SceneGraph::GetInstance().GetCmpntTransformData(m_ModelId);
    return Transform.matrix;
}

void Camera::CreateModel()
{
    CModel::S_MODEL_DESC desc;
    desc.strName = "Camera";
    desc.S_MODEL_COLUMN_DESC.start = glm::vec3(0.0f, 0.0f, 0.0f);
    desc.S_MODEL_COLUMN_DESC.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    desc.S_MODEL_COLUMN_DESC.length = 1.0f;

    std::shared_ptr<CModel> model = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, desc);
    if (nullptr == model) {
        PRINTLOG("Fail to create camera model");
        return;
    }
    m_ModelId = SceneGraph::GetInstance().CreateModel(*model.get());

    glm::mat4 invView = glm::inverse(glm::lookAt(m_Position, m_Target, m_Up));
    SetTransformData(invView);

    glm::mat4 view = glm::lookAt(m_Position, m_Target, m_Up);
    m_position = view * glm::vec4(m_Position, 1.0f);
    m_target = view * glm::vec4(m_Target, 1.0f);
    m_up = view * glm::vec4(m_Up, 1.0f);
}
