#include "CSession.h"
#include "../CEngine.h"
#include <CUtils.h>
#include "CSessionManager.h"
#include "../Render/CRenderSystem.h"

CSession::CSession()
{
	m_OperatorAction = E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_MOVING;
	m_bLeftMouseMoved = false;
	m_AxisTransform = glm::vec3(1.0f, 0.0f, 0.0f);
}

bool CSession::Initialize()
{
	CSessionManager::SwitchCurrentSession(this);
	if (!InitializeCamera()) {
		PRINTLOG("Fail to initialize Camera");
		return false;
	}
	if (!InitializeFramebuffer()) {
		PRINTLOG("Fail to initialize Framebuffer");
		return false;
	}
	if (!InitializeMerterial()) {
		PRINTLOG("Fail to initialize Material");
		return false;
	}
	// 改为在DllMain里初始化
	//return CEngine::GetInstance().Initialize();
	return true;
}

void CSession::UnInitialize()
{
	CSessionManager::SwitchCurrentSession(this);
}

void CSession::Render()
{
	CSessionManager::SwitchCurrentSession(this);

	if (nullptr == m_Framebuffer) {
		return;
	}

	m_Framebuffer->Bind();

	glViewport(0, 0, m_camera->GetWidth(), m_camera->GetHeight());

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 通过UBO设置投影矩阵和视图矩阵
	std::shared_ptr<CUniformBuffer> ubo = CResourceManager::AquireUnifrombuffer(UBO_BINDINGPOINT_MVP);
	if (nullptr != ubo) {
		ubo->SendData(&m_camera->GetProjection(), sizeof(glm::mat4), offsetof(CShader::S_SHADER_MVP_MATRIX, projection));
		ubo->SendData(&m_camera->GetView(), sizeof(glm::mat4), offsetof(CShader::S_SHADER_MVP_MATRIX, view));
	}
	
	CRenderSystem::CRenderContext context;
	context.m_Camera = m_camera;
	//context.m_Material = m_MaterialRender;
	context.m_set_SelectedId = m_set_SelectedId;
	// 跟新位置
	CRenderSystem::Initialize(context, CSceneGraphManager::GetInstance());
	CRenderSystem::Update(CSceneGraphManager::GetInstance());
	CRenderSystem::Render(context, CSceneGraphManager::GetInstance());

	m_Framebuffer->UnBind();
}


unsigned int CSession::GetRenderTextureId()
{
	if (nullptr != m_Framebuffer && 0 < m_Framebuffer->GetTextureSize()) {
		return m_Framebuffer->GetTexture(0)->GetID();
	}
	return 0;
}

void CSession::Resize(int width, int height)
{
	CSessionManager::SwitchCurrentSession(this);
	if (nullptr != m_Framebuffer) {
		m_Framebuffer->Resize(width, height);
	}
	if (nullptr != m_camera) {
		m_camera->Resize(width, height);
	}
}

bool CSession::MergeModel()
{
	CSessionManager::SwitchCurrentSession(this);
	// 用户是否有且只选择了两个对象
	if (2 != m_set_SelectedId.size()) {
		PRINTLOG("Selected model mush equal 2(current is %u)", m_set_SelectedId.size());
		return false;
	}
	return CEngine::GetInstance().MergeModel(*m_set_SelectedId.begin(), *std::next(m_set_SelectedId.begin()));
}

void CSession::OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y)
{
	CSessionManager::SwitchCurrentSession(this);
	switch (key) {
	case E_MOUSE_BUTTON_LEFT:
		if (E_MOUSE_ACTION_PRESS == action) {
			OnMouseLeftPress(x, y);
		}
		else if (E_MOUSE_ACTION_RELEASE == action) {
			OnMouseLeftRelease(x, y);
		}
		else if (E_MOUSE_ACTION_MOVING == action) {
			OnMouseLeftMoving(x, y);
		}
		break;
	case E_MOUSE_BUTTON_RIGHT:
		if (E_MOUSE_ACTION_RELEASE == action) {

		}
		break;
	case E_MOUSE_BUTTON_MIDDLE:
		break;
	}
	if (E_MOUSE_BUTTON_UNKOWN == key && E_MOUSE_ACTION_WHEEL == action) {
		OnMouseWheel(x / 100.0f);
	}
}

glm::vec3 CSession::GetRayDirection(
	int width, 
	int height, 
	double mouseX, 
	double mouseY, 
	const glm::mat4& view, 
	const glm::mat4& projection
)
{
	// 1. 获取视口定义 [x, y, width, height]
	glm::vec4 viewport = glm::vec4(0, 0, width, height);

	// 2. 转换鼠标坐标
	// GLFW 等库的 y 是自顶向下的，而 OpenGL 的 unProject 预期 y 是自底向上的
	float winX = (float)mouseX;
	float winY = (float)height - (float)mouseY;

	// 3. 计算近裁剪面上的点 (z = 0.0 在 unProject 中映射到 NDC 的 -1)
	glm::vec3 nearPos = glm::unProject(
		glm::vec3(winX, winY, 0.0f),
		view,
		projection,
		viewport
	);

	// 4. 计算远裁剪面上的点 (z = 1.0 在 unProject 中映射到 NDC 的 1)
	glm::vec3 farPos = glm::unProject(
		glm::vec3(winX, winY, 1.0f),
		view,
		projection,
		viewport
	);

	//m_pModelColumn = CObjectManager::GetInstance().GetColumnModel(farPos, nearPos, 0.01f);
	// 5. 计算方向向量并归一化
	return glm::normalize(farPos - nearPos);
}

// 输入：鼠标射线向量 v, 相机位置 O, 模型当前位置 P0, 轴向量 u
// 输出：模型沿轴移动后的距离
float CSession::GetMovementOnAxis(glm::vec3 rayDir, glm::vec3 camPos, glm::vec3 modelPos, glm::vec3 axis)
{
	// 确保方向向量都是单位向量
	glm::vec3 u = glm::normalize(axis);
	glm::vec3 v = glm::normalize(rayDir);
	glm::vec3 r = camPos - modelPos; // 注意符号，这里对应推导中的 O - P0

	float b = glm::dot(u, v);
	float d = glm::dot(u, r);
	float e = glm::dot(v, r);

	float denominator = 1.0f - b * b;

	// 如果分母接近 0，说明射线与轴平行，无法产生有效位移
	if (glm::abs(denominator) < 0.0001f) {
		return 0.0f;
	}

	return (b * e - d) / denominator;
}

bool CSession::GetRayPlaneIntersection(
	const glm::vec3& rayOrigin,
	const glm::vec3& rayDir,
	const glm::vec3& planePoint,
	const glm::vec3& planeNormal,
	glm::vec3& outIntersection)
{
	float denom = glm::dot(planeNormal, rayDir);
	// 确保射线不与平面平行（阈值可微调）
	if (std::abs(denom) > 1e-6f) {
		float t = glm::dot(planePoint - rayOrigin, planeNormal) / denom;
		if (t >= 0) {
			outIntersection = rayOrigin + t * rayDir;
			return true;
		}
	}
	return false;
}

float CSession::CalculateAngleOnPlane(
	const glm::vec3& intersection,
	const glm::vec3& centerPoint,
	const glm::vec3& rotationAxis)
{
	glm::vec3 normAxis = glm::normalize(rotationAxis);

	// 1. 构建平面局部坐标系的基向量 (Tangent 和 Bitangent)
	glm::vec3 tangentX;
	if (std::abs(normAxis.y) < 0.9f)
		tangentX = glm::normalize(glm::cross(normAxis, glm::vec3(0, 1, 0)));
	else
		tangentX = glm::normalize(glm::cross(normAxis, glm::vec3(1, 0, 0)));

	glm::vec3 tangentY = glm::cross(normAxis, tangentX);

	// 2. 计算交点相对于中心点的偏移向量
	glm::vec3 localVec = intersection - centerPoint;

	// 3. 投影到 X 和 Y 轴得到 2D 坐标
	float x = glm::dot(localVec, tangentX);
	float y = glm::dot(localVec, tangentY);

	// 4. 返回方位角 (范围 -PI 到 PI)
	return std::atan2(y, x);
}

void CSession::OnModelSelectedAction(int x, int y)
{
	if (nullptr != m_Framebuffer && nullptr != m_camera &&
		0 < m_Framebuffer->GetTextureSize())
	{
		unsigned int nSelectedId = 0;

		m_Framebuffer->Bind();

		glViewport(0, 0, m_camera->GetWidth(), m_camera->GetHeight());
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 通过UBO设置投影矩阵和视图矩阵
		std::shared_ptr<CUniformBuffer> ubo = CResourceManager::AquireUnifrombuffer(UBO_BINDINGPOINT_MVP);
		if (nullptr != ubo) {
			ubo->SendData(&m_camera->GetProjection(), sizeof(glm::mat4), offsetof(CShader::S_SHADER_MVP_MATRIX, projection));
			ubo->SendData(&m_camera->GetView(), sizeof(glm::mat4), offsetof(CShader::S_SHADER_MVP_MATRIX, view));
		}

		CRenderSystem::CRenderContext context;
		context.m_Camera = m_camera;
		context.m_Material = m_MaterialSelect;
		context.m_RenderID = true;
		// 跟新位置
		CRenderSystem::Initialize(context, CSceneGraphManager::GetInstance());
		CRenderSystem::Update(CSceneGraphManager::GetInstance());
		CRenderSystem::Render(context, CSceneGraphManager::GetInstance());

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, m_Framebuffer->GetTexture(0)->GetID(), 0);
		// 读取像素
		glReadPixels((GLint)x, (GLint)(m_camera->GetHeight() - y),
			1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &nSelectedId);

		//PRINTLOG("nSelectedId=%u[%d,%d]", nSelectedId, x, m_camera->GetWidth());
		if (CSceneGraphManager::GetInstance().EntityIsValid((entt::entity)nSelectedId)) {
			m_set_SelectedId.insert((entt::entity)nSelectedId);
		}
		else {
			m_set_SelectedId.clear();
		}
		m_Framebuffer->UnBind();
	}
}

void CSession::OnModelTranslateActionBegin(int x, int y)
{
	if (1 != m_set_SelectedId.size()) {
		return;
	}
	entt::entity pickedId = *m_set_SelectedId.begin();
	if (!CSceneGraphManager::GetInstance().HaveAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId)) {
		return;
	}
	CSceneGraphComponent::S_TRANSFORM_INFO& Transform = CSceneGraphManager::GetInstance()
		.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId);

	m_ModelPosition = Transform.translation;
	glm::vec3 rayDir = GetRayDirection(m_camera->GetWidth(), m_camera->GetHeight(), x, y, m_camera->GetView(), m_camera->GetProjection());
	m_StartPosition = GetMovementOnAxis(rayDir, m_camera->GetPosition(), m_ModelPosition, m_AxisTransform);
}

void CSession::OnModelTranslateActionEnd(int x, int y)
{
}

void CSession::OnModelTranslateActionIng(int x, int y)
{
	if (1 != m_set_SelectedId.size()) {
		return;
	}
	entt::entity pickedId = *m_set_SelectedId.begin();
	if (!CSceneGraphManager::GetInstance().HaveAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId)) {
		return;
	}
	CSceneGraphComponent::S_TRANSFORM_INFO& Transform = CSceneGraphManager::GetInstance()
		.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId);

	m_bLeftMouseMoved = true;
	glm::vec3 rayDir = GetRayDirection(m_camera->GetWidth(), m_camera->GetHeight(), x, y, m_camera->GetView(), m_camera->GetProjection());
	float current = GetMovementOnAxis(rayDir, m_camera->GetPosition(), m_ModelPosition, m_AxisTransform);
	Transform.translation = m_ModelPosition + m_AxisTransform * (m_StartPosition - current);
}

void CSession::OnModelRotateActionBegin(int x, int y)
{
	if (1 != m_set_SelectedId.size()) {
		return;
	}
	entt::entity pickedId = *m_set_SelectedId.begin();
	if (!CSceneGraphManager::GetInstance().HaveAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId)) {
		return;
	}
	CSceneGraphComponent::S_TRANSFORM_INFO& Transform = CSceneGraphManager::GetInstance()
		.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId);

	glm::vec3 rayDir = GetRayDirection(m_camera->GetWidth(), m_camera->GetHeight(), x, y, m_camera->GetView(), m_camera->GetProjection());
	glm::vec3 intersection;
	m_ModelPosition = Transform.translation;

	if (GetRayPlaneIntersection(m_camera->GetPosition(), rayDir, m_ModelPosition, m_AxisTransform, intersection)) {
		// A. 保存点击时的初始方位角
		m_StartRotateAngle = CalculateAngleOnPlane(intersection, m_ModelPosition, m_AxisTransform);
	}
}

void CSession::OnModelRotateActionEnd(int x, int y)
{

}

void CSession::OnModelRotateActionIng(int x, int y)
{
	if (1 != m_set_SelectedId.size()) {
		return;
	}
	entt::entity pickedId = *m_set_SelectedId.begin();
	if (!CSceneGraphManager::GetInstance().HaveAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId)) {
		return;
	}
	CSceneGraphComponent::S_TRANSFORM_INFO& Transform = CSceneGraphManager::GetInstance()
		.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId);

	// 1. 获取当前射线
	glm::vec3 rayDir = GetRayDirection(m_camera->GetWidth(), m_camera->GetHeight(), x, y, m_camera->GetView(), m_camera->GetProjection());
	glm::vec3 intersection;

	if (GetRayPlaneIntersection(m_camera->GetPosition(), rayDir, m_ModelPosition, m_AxisTransform, intersection)) {
		// C. 计算当前的方位角
		float currentAngle = CalculateAngleOnPlane(intersection, m_ModelPosition, m_AxisTransform);

		// D. 计算角度差值
		float deltaAngle = currentAngle - m_StartRotateAngle;
		// E. 构造增量四元数，并叠加到初始姿态上
		glm::quat deltaQuat = glm::angleAxis(deltaAngle, glm::normalize(m_AxisTransform));
		Transform.rotation = deltaQuat;
		m_bLeftMouseMoved = true;
	}
}

void CSession::OnMouseWheel(float delta)
{
	if (1 != m_set_SelectedId.size()) {
		return;
	}
	entt::entity pickedId = *m_set_SelectedId.begin();
	if (!CSceneGraphManager::GetInstance().HaveAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId)) {
		return;
	}
	// 前滚放大(delta>0)，后滚缩小(delta<0)
	float scaleDelta = delta * 0.1f;
	CSceneGraphComponent::S_TRANSFORM_INFO& Transform = CSceneGraphManager::GetInstance()
		.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(pickedId);

	glm::vec3 currentScale = Transform.scale;
	float newScale = currentScale.x + scaleDelta;
	if (newScale < 0.1f) newScale = 0.1f;
	if (newScale > 10.0f) newScale = 10.0f;
	Transform.scale = glm::vec3(newScale, newScale, newScale);
	//PRINTLOG("%u[%f,%f,%f]", pickedId, Transform.scale.x, Transform.scale.y, Transform.scale.z);
}

void CSession::OnMouseLeftPress(int x, int y)
{
	m_OperatorAction = E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_ROTATE;
	if (E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_MOVING == m_OperatorAction) {
		OnModelTranslateActionBegin(x, y);
	}
	else if (E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_ROTATE == m_OperatorAction) {
		OnModelRotateActionBegin(x, y);
	}
}

void CSession::OnMouseLeftRelease(int x, int y)
{
	if (!m_bLeftMouseMoved) {
		OnModelSelectedAction(x, y);
		return;
	}
	if (E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_MOVING == m_OperatorAction) {
		OnModelTranslateActionEnd(x, y);
	}
	else if (E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_ROTATE == m_OperatorAction) {
		OnModelRotateActionEnd(x, y);
	}

	m_bLeftMouseMoved = false;
}

void CSession::OnMouseLeftMoving(int x, int y)
{
	if (E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_MOVING == m_OperatorAction) {
		OnModelTranslateActionIng(x, y);
	}
	else if (E_OPERATOR_ACTION_TYPE::E_OPERATOR_ACTION_ROTATE == m_OperatorAction) {
		OnModelRotateActionIng(x, y);
	}
}

bool CSession::InitializeCamera()
{
	m_camera = std::make_shared<Camera>();
	if (nullptr == m_camera) {
		PRINTLOG("Fail to create camera");
		return false;
	}
	return true;
}

bool CSession::InitializeFramebuffer()
{
	CFramebuffer::S_FRAMEBUFFER_DESC desc;
	desc.width = m_camera->GetWidth();
	desc.height = m_camera->GetHeight();
	desc.vecTexture.push_back(CTexture::S_TEXTURE_DESC());
	desc.vecTexture[0].width = desc.width;
	desc.vecTexture[0].height = desc.height;
	desc.vecTexture[0].Format = GL_RGBA;
	m_Framebuffer = CResourceManager::AquireFramebuffer(desc);
	if (nullptr == m_Framebuffer) {
		PRINTLOG("Fail to create Framebuffer");
		return false;
	}
	return true;
}

bool CSession::InitializeMerterial()
{
	CMaterial::S_MATERIAL_DESC desc;
	desc.hasPosition = true;
	desc.hasTexCoords = true;
	desc.hasSelected = true;
	m_MaterialRender = CMaterialSystem::AquireMaterial(desc);
	if (nullptr == m_MaterialRender) {
		PRINTLOG("Fail create render Material");
		return false;
	}
	desc = CMaterial::S_MATERIAL_DESC();
	desc.hasPosition = true;
	desc.renderID = true;
	m_MaterialSelect = CMaterialSystem::AquireMaterial(desc);
	if (nullptr == m_MaterialSelect) {
		PRINTLOG("Fail create select Material");
		return false;
	}
	return true;
}