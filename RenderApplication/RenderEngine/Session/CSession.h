#pragma once
#include <set>
#include <memory>
#include <entt/entt.hpp>
#include "../SceneGraph/Camera.h"
#include "../IEngineInterface.h"
#include "../Resource/CResourceManager.h"
#include "../Material/CMaterialSystem.h"

class CSession : public ISessionInterface
{
public:
	enum class E_OPERATOR_ACTION_TYPE
	{
		E_OPERATOR_ACTION_VALIDATE,	// 无效操作
		E_OPERATOR_ACTION_SELECTED,// 选择
		E_OPERATOR_ACTION_MOVING,	// 移动
		E_OPERATOR_ACTION_ROTATE,	// 旋转
	};
public:
	CSession();
public:
	bool Initialize() override;
	void UnInitialize() override;
	void Render() override;
	unsigned int GetRenderTextureId() override;
	void Resize(int width, int height) override;
	bool MergeModel() override;
public:
	void OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y) override;
private:
	glm::vec3 GetRayDirection(
		int width,
		int height,
		double mouseX,
		double mouseY,
		const glm::mat4& view,
		const glm::mat4& projection
	);
	float GetMovementOnAxis(glm::vec3 rayDir, glm::vec3 camPos, glm::vec3 modelPos, glm::vec3 axis);
	bool GetRayPlaneIntersection(
		const glm::vec3& rayOrigin,
		const glm::vec3& rayDir,
		const glm::vec3& planePoint,
		const glm::vec3& planeNormal,
		glm::vec3& outIntersection);
	float CalculateAngleOnPlane(
		const glm::vec3& intersection,
		const glm::vec3& centerPoint,
		const glm::vec3& rotationAxis);
private:
	void OnModelSelectedAction(int x, int y);
	void OnModelTranslateActionBegin(int x, int y);
	void OnModelTranslateActionEnd(int x, int y);
	void OnModelTranslateActionIng(int x, int y);

	void OnModelRotateActionBegin(int x, int y);
	void OnModelRotateActionEnd(int x, int y);
	void OnModelRotateActionIng(int x, int y);
private:
	void OnMouseWheel(float delta);
	void OnMouseLeftPress(int x, int y);
	void OnMouseLeftRelease(int x, int y);
	void OnMouseLeftMoving(int x, int y);
public:
	std::shared_ptr<Camera> GetCamera() const { return m_camera; }
	std::shared_ptr<CFramebuffer> GetFramebuffer() const { return m_Framebuffer; }
	//std::shared_ptr<CMaterial> GetMaterial()const { return m_Material; }

private:
	bool InitializeCamera();
	bool InitializeFramebuffer();
	bool InitializeMerterial();

private:
	std::shared_ptr<Camera>			m_camera;
	std::shared_ptr<CFramebuffer>	m_Framebuffer;
	std::shared_ptr<CMaterial>		m_MaterialRender;
	std::shared_ptr<CMaterial>		m_MaterialSelect;
	std::set<entt::entity>			m_set_SelectedId;


	bool							m_bLeftMouseMoved;
	E_OPERATOR_ACTION_TYPE			m_OperatorAction;
	float							m_StartPosition;
	glm::vec3						m_ModelPosition;
	glm::vec3						m_AxisTransform;
	float							m_StartRotateAngle;

};