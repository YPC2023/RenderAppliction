#include "CoordinateSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SceneGraph.h"
#include <CUtils.h>

CoordinateSystem::CoordinateSystem()
{
	m_ModelId = SetupCoondinateSystem();
	// 给场景图的坐标轴节点设置标志
	SetupCoordinateFlag();
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
	// 创建XY圆环弧
	entt::entity entityXY0 = CreateCoordinateArcXY0();
	if (entt::null == entityXY0) {
		PRINTLOG("Fial to create coordinate XY0");
		return entt::null;
	}
	// 创建XY圆环弧
	entt::entity entityXY1 = CreateCoordinateArcXY1();
	if (entt::null == entityXY1) {
		PRINTLOG("Fial to create coordinate XY1");
		return entt::null;
	}
	// 创建XY圆环弧
	entt::entity entityXY2 = CreateCoordinateArcXY2();
	if (entt::null == entityXY2) {
		PRINTLOG("Fial to create coordinate XY2");
		return entt::null;
	}
	// 创建XY圆环弧
	entt::entity entityXY3 = CreateCoordinateArcXY3();
	if (entt::null == entityXY3) {
		PRINTLOG("Fial to create coordinate XY3");
		return entt::null;
	}
	// 绑定x-y
	SceneGraph::GetInstance().BindModel(entityX, entityY, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXY0, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXY1, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXY2, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXY3, false);

	// 创建z轴
	entt::entity entityZ = CreateCoordinateAxesZ();
	if (entt::null == entityZ) {
		PRINTLOG("Fail to create coordinate Z");
		return entt::null;
	}
	// 创建YZ圆环弧
	entt::entity entityYZ0 = CreateCoordinateArcYZ0();
	if (entt::null == entityYZ0) {
		PRINTLOG("Fial to create coordinate YZ0");
		return entt::null;
	}
	// 创建YZ圆环弧
	entt::entity entityYZ1 = CreateCoordinateArcYZ1();
	if (entt::null == entityYZ1) {
		PRINTLOG("Fial to create coordinate YZ1");
		return entt::null;
	}
	// 创建YZ圆环弧
	entt::entity entityYZ2 = CreateCoordinateArcYZ2();
	if (entt::null == entityYZ2) {
		PRINTLOG("Fial to create coordinate YZ2");
		return entt::null;
	}
	// 创建YZ圆环弧
	entt::entity entityYZ3 = CreateCoordinateArcYZ3();
	if (entt::null == entityYZ3) {
		PRINTLOG("Fial to create coordinate YZ3");
		return entt::null;
	}
	// 绑定x-z
	SceneGraph::GetInstance().BindModel(entityX, entityZ, false);
	SceneGraph::GetInstance().BindModel(entityX, entityYZ0, false);
	SceneGraph::GetInstance().BindModel(entityX, entityYZ1, false);
	SceneGraph::GetInstance().BindModel(entityX, entityYZ2, false);
	SceneGraph::GetInstance().BindModel(entityX, entityYZ3, false);


	// 创建YZ圆环弧
	entt::entity entityXZ0 = CreateCoordinateArcXZ0();
	if (entt::null == entityXZ0) {
		PRINTLOG("Fial to create coordinate XZ0");
		return entt::null;
	}
	// 创建XZ圆环弧
	entt::entity entityXZ1 = CreateCoordinateArcXZ1();
	if (entt::null == entityXZ1) {
		PRINTLOG("Fial to create coordinate XZ1");
		return entt::null;
	}
	// 创建XZ圆环弧
	entt::entity entityXZ2 = CreateCoordinateArcXZ2();
	if (entt::null == entityXZ2) {
		PRINTLOG("Fial to create coordinate XZ2");
		return entt::null;
	}
	// 创建XZ圆环弧
	entt::entity entityXZ3 = CreateCoordinateArcXZ3();
	if (entt::null == entityXZ3) {
		PRINTLOG("Fial to create coordinate XZ3");
		return entt::null;
	}

	SceneGraph::GetInstance().BindModel(entityX, entityXZ0, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXZ1, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXZ2, false);
	SceneGraph::GetInstance().BindModel(entityX, entityXZ3, false);
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

entt::entity CoordinateSystem::CreateCoordinateArcXY0()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.startAngle = 0.0f;
	desc.endAngle = 90.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXY0 = std::make_shared<CoordinateArc>(desc, "XY0");
	if (nullptr == m_CoordArcXY0) {
		PRINTLOG("Fail to create coordinate XY0");
		return entt::null;
	}
	return m_CoordArcXY0->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcXY1()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.startAngle = 90.0f;
	desc.endAngle = 180.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXY1 = std::make_shared<CoordinateArc>(desc, "XY1");
	if (nullptr == m_CoordArcXY1) {
		PRINTLOG("Fail to create coordinate XY1");
		return entt::null;
	}
	return m_CoordArcXY1->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcXY2()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.startAngle = 180.0f;
	desc.endAngle = 270.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXY2 = std::make_shared<CoordinateArc>(desc, "XY2");
	if (nullptr == m_CoordArcXY2) {
		PRINTLOG("Fail to create coordinate XY2");
		return entt::null;
	}
	return m_CoordArcXY2->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcXY3()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.startAngle = 270.0f;
	desc.endAngle = 360.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXY3 = std::make_shared<CoordinateArc>(desc, "XY3");
	if (nullptr == m_CoordArcXY3) {
		PRINTLOG("Fail to create coordinate XY3");
		return entt::null;
	}
	return m_CoordArcXY3->m_ModelId;
}



entt::entity CoordinateSystem::CreateCoordinateArcYZ0()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.refVector = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.startAngle = 0.0f;
	desc.endAngle = 90.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcYZ0 = std::make_shared<CoordinateArc>(desc, "YZ0");
	if (nullptr == m_CoordArcYZ0) {
		PRINTLOG("Fail to create coordinate YZ0");
		return entt::null;
	}
	return m_CoordArcYZ0->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcYZ1()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.refVector = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.startAngle = 90.0f;
	desc.endAngle = 180.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcYZ1 = std::make_shared<CoordinateArc>(desc, "YZ1");
	if (nullptr == m_CoordArcYZ1) {
		PRINTLOG("Fail to create coordinate YZ1");
		return entt::null;
	}
	return m_CoordArcYZ1->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcYZ2()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.refVector = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.startAngle = 180.0f;
	desc.endAngle = 270.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcYZ2 = std::make_shared<CoordinateArc>(desc, "YZ2");
	if (nullptr == m_CoordArcYZ2) {
		PRINTLOG("Fail to create coordinate YZ2");
		return entt::null;
	}
	return m_CoordArcYZ2->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcYZ3()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.refVector = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.startAngle = 270.0f;
	desc.endAngle = 360.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcYZ3 = std::make_shared<CoordinateArc>(desc, "YZ3");
	if (nullptr == m_CoordArcYZ3) {
		PRINTLOG("Fail to create coordinate YZ3");
		return entt::null;
	}
	return m_CoordArcYZ3->m_ModelId;
}


entt::entity CoordinateSystem::CreateCoordinateArcXZ0()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.startAngle = 0.0f;
	desc.endAngle = 90.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXZ0 = std::make_shared<CoordinateArc>(desc, "XZ0");
	if (nullptr == m_CoordArcXZ0) {
		PRINTLOG("Fail to create coordinate XZ0");
		return entt::null;
	}
	return m_CoordArcXZ0->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcXZ1()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.eColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.startAngle = 90.0f;
	desc.endAngle = 180.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXZ1 = std::make_shared<CoordinateArc>(desc, "XZ1");
	if (nullptr == m_CoordArcXZ1) {
		PRINTLOG("Fail to create coordinate XZ1");
		return entt::null;
	}
	return m_CoordArcXZ1->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcXZ2()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.eColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.startAngle = 180.0f;
	desc.endAngle = 270.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXZ2 = std::make_shared<CoordinateArc>(desc, "XZ2");
	if (nullptr == m_CoordArcXZ2) {
		PRINTLOG("Fail to create coordinate XZ2");
		return entt::null;
	}
	return m_CoordArcXZ2->m_ModelId;
}

entt::entity CoordinateSystem::CreateCoordinateArcXZ3()
{
	CoordinateArc::S_COORDS_DESC desc;
	desc.center = glm::vec3(0.0f);
	desc.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.refVector = glm::vec3(1.0f, 0.0f, 0.0f);
	desc.sColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.eColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.startAngle = 270.0f;
	desc.endAngle = 360.0f;
	desc.outerRadius = 3.0f;
	desc.ringWidth = 0.1f;
	m_CoordArcXZ3 = std::make_shared<CoordinateArc>(desc, "XZ3");
	if (nullptr == m_CoordArcXZ3) {
		PRINTLOG("Fail to create coordinate XZ3");
		return entt::null;
	}
	return m_CoordArcXZ3->m_ModelId;
}


void CoordinateSystem::SetupCoordinateFlag()
{
	std::set<entt::entity> setNode = SceneGraph::GetInstance().GetModelTransformComponents(m_ModelId);
	for (auto entity : setNode) {
		SceneGraph::GetInstance().AppendComponent<SGCmpnt::S_CMPNT_COORDINATE_FLAG>(entity);
	}
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
}