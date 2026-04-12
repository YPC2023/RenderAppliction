#include "CoordinateSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SceneGraph.h"
#include <CUtils.h>

CoordinateSystem::CoordinateSystem()
{
	m_ModelId = SetupCoondinateSystem();
	BindCallback();
}

CoordinateSystem::~CoordinateSystem()
{

}

entt::entity CoordinateSystem::SetupCoondinateSystem()
{
	// 创建x轴
	entt::entity entityX = CreateCoordinateAxesX();
	if (entt::null == entityX) {
		PRINTLOG("Fail to create coordinate X");
		return entt::null;
	}
	// 创建y轴
	entt::entity entityY = CreateCoordinateAxesY();
	if (entt::null == entityY) {
		PRINTLOG("Fail to create coordinate Y");
		return entt::null;
	}
	// 绑定x-y
	SceneGraph::GetInstance().BindModel(entityX, entityY, false);

	// 创建z轴
	entt::entity entityZ = CreateCoordinateAxesZ();
	if (entt::null == entityZ) {
		PRINTLOG("Fail to create coordinate Z");
		return entt::null;
	}
	// 绑定x-z
	SceneGraph::GetInstance().BindModel(entityX, entityZ, false);

	return entityX;
}

void CoordinateSystem::BindModel(entt::entity entity)
{
	if (entt::null == entity || entt::null == m_ModelId) {
		return;
	}
	ASSERT(SceneGraph::GetInstance().HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	m_BindId = entity;

	auto& CoordTransformData = SceneGraph::GetInstance().GetCmpntTransformData(m_ModelId);
	const auto& ModelTransformData = SceneGraph::GetInstance().GetCmpntTransformData(entity);
	CoordTransformData.translation.set(ModelTransformData.translation.get());
	CoordTransformData.rotation.set(ModelTransformData.rotation.get());
	CoordTransformData.scale.set(ModelTransformData.scale.get());
	//m_CoordAxesX->SetTransformData(TransformData);
	//m_CoordAxesY->SetTransformData(TransformData);
	//m_CoordAxesZ->SetTransformData(TransformData);
}

void CoordinateSystem::UnBindModel()
{
	m_BindId = entt::null;
}

entt::entity CoordinateSystem::CreateCoordinateAxesX()
{
	CoordinateAxes::S_COORDS_DESC desc;
	desc.length = 5.0f;
	desc.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	m_CoordAxesX = std::make_shared<CoordinateAxes>(desc, "X");
	if (nullptr == m_CoordAxesX) {
		PRINTLOG("Fail to create coordinate X");
		return entt::null;
	}
	return m_CoordAxesX->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateAxesY()
{
	CoordinateAxes::S_COORDS_DESC desc;
	desc.length = 5.0f;
	desc.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	m_CoordAxesY = std::make_shared<CoordinateAxes>(desc, "Y");
	if (nullptr == m_CoordAxesY) {
		PRINTLOG("Fail to create coordinate Y");
		return entt::null;
	}
	return m_CoordAxesY->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateAxesZ()
{
	CoordinateAxes::S_COORDS_DESC desc;
	desc.length = 5.0f;
	desc.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	m_CoordAxesZ = std::make_shared<CoordinateAxes>(desc, "Z");
	if (nullptr == m_CoordAxesZ) {
		PRINTLOG("Fail to create coordinate Z");
		return entt::null;
	}
	return m_CoordAxesZ->m_ModelId;
}

void CoordinateSystem::BindCallback()
{
	if (entt::null != m_ModelId) 
	{
		auto& Tranform = SceneGraph::GetInstance().GetCmpntTransformData(m_ModelId);
		Tranform.matrix.setCallback(Callback);
		Tranform.matrix.setPayload((void*)this);
	}
	
}

void CoordinateSystem::Callback(const glm::mat4& old_value, const glm::mat4& new_value, void* payload)
{
	if (0 == payload) {
		return;
	}
	CoordinateSystem* p = (CoordinateSystem*)payload;
	PRINTLOG("%p", p);
}