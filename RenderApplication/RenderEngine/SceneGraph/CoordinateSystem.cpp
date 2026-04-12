#include "CoordinateSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SceneGraph.h"
#include <CUtils.h>

CoordinateSystem::CoordinateSystem()
{
	SetupCoondinateSystem();
}

CoordinateSystem::~CoordinateSystem()
{

}

void CoordinateSystem::SetupCoondinateSystem()
{
	CoordinateAxes::S_COORDS_DESC desc;
	desc.length = 5.0f;

	desc.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	desc.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	m_CoordAxesX = std::make_shared<CoordinateAxes>(desc, "X");
	if (nullptr == m_CoordAxesX) {
		PRINTLOG("Fail to create coordinate X");
		return;
	}
	desc.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	desc.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	m_CoordAxesY = std::make_shared<CoordinateAxes>(desc, "Y");
	if (nullptr == m_CoordAxesY) {
		PRINTLOG("Fail to create coordinate Y");
		return;
	}

	SceneGraph::GetInstance().BindModel(m_CoordAxesX->m_ModelId, m_CoordAxesY->m_ModelId);

	desc.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	desc.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	m_CoordAxesZ = std::make_shared<CoordinateAxes>(desc, "Z");
	if (nullptr == m_CoordAxesZ) {
		PRINTLOG("Fail to create coordinate Z");
		return;
	}

	SceneGraph::GetInstance().BindModel(m_CoordAxesX->m_ModelId, m_CoordAxesZ->m_ModelId);
}