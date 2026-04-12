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
    m_Matrix = glm::mat4(1.0f);

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
    CalculateView();
    CalculateProjection();
}

void Camera::SetupCamera()
{
    UpdateCamera();
    CreateModel();
}

void Camera::CalculateProjection()
{
    m_Projection = glm::perspective(glm::radians(m_Fov), ((float)m_nWidth / m_nHeight), m_Near, m_Far);
}

void Camera::CalculateView()
{
    glm::vec3 Position = glm::vec3(m_Matrix * glm::vec4(m_Position, 1.0f));
    glm::vec3 Target = glm::vec3(m_Matrix * glm::vec4(m_Target, 1.0f));
    m_View = glm::lookAt(Position, Target, m_Up);
}

void Camera::CreateModel()
{
    CModel::S_MODEL_DESC desc;
    desc.strName = "Camera";
    //desc.S_MODEL_COLUMN_DESC.start = m_Position;
    //desc.S_MODEL_COLUMN_DESC.normal = glm::normalize(m_Position - m_Target);
    //desc.S_MODEL_COLUMN_DESC.length = 1.0f;

    std::shared_ptr<CModel> model = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, desc);
    if (nullptr == model) {
        PRINTLOG("Fail to create camera model");
        return;
    }
    m_ModelId = SceneGraph::GetInstance().CreateModel(*model.get());

    
    // 计算旋转向量
    glm::vec3 zAxis = glm::normalize(m_Position - m_Target); // 摄像机正后方 (+Z)
    glm::vec3 xAxis = glm::normalize(glm::cross(m_Up, zAxis)); // 摄像机右方 (+X)
    glm::vec3 yAxis = glm::cross(zAxis, xAxis); // 摄像机上方 (+Y)
    glm::mat3 rotationMat;
    rotationMat[0] = xAxis; // 第一列
    rotationMat[1] = yAxis; // 第二列
    rotationMat[2] = zAxis; // 第三列
    // 直接转换为四元数
    glm::quat rotationQuat = glm::quat_cast(rotationMat);
    
    auto& Transform = SceneGraph::GetInstance().GetCmpntTransformData(m_ModelId);
    Transform.translation.set(m_Position);
    Transform.rotation.set(rotationQuat);
    Transform.matrix.setCallback(Callback);
    Transform.matrix.setPayload((void*)this);
}

void Camera::Callback(const glm::mat4& old_value, const glm::mat4& new_value, void* payload)
{
    if (0 == payload) {
        return;
    }
    Camera* p = (Camera*)payload;
    p->m_Matrix = new_value;
    p->UpdateCamera();
}