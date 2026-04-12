#include "CoordinateAxes.h"
#include "SceneGraph.h"
#include <CUtils.h>

CoordinateAxes::CoordinateAxes(const S_COORDS_DESC& desc, const std::string& strName)
	: m_desc(desc), m_strName(strName)
{
	m_ModelId = SetupCoordinateAxes();
}

CoordinateAxes::~CoordinateAxes()
{
	std::vector<entt::entity> vecNode = SceneGraph::GetInstance().GetModelTransformComponents(m_ModelId);
	for (auto entity : vecNode) {
		SceneGraph::GetInstance().RemoveNode(entity);
	}
}

entt::entity CoordinateAxes::SetupCoordinateAxes()
{
	glm::vec3 start = m_desc.start;
	glm::vec3 end = m_desc.start + (m_desc.length * glm::normalize(m_desc.normal));
	// ×ø±êÖáÔ²ÖùÌå
	CModel::S_MODEL_DESC descColumn;
	descColumn.strName = (std::string("COLUMN_") + m_strName);
	descColumn.S_MODEL_COLUMN_DESC.start = start;
	descColumn.S_MODEL_COLUMN_DESC.end = end;
	descColumn.S_MODEL_COLUMN_DESC.radius = m_desc.radius;
	descColumn.S_MODEL_COLUMN_DESC.sColor = m_desc.color;
	descColumn.S_MODEL_COLUMN_DESC.eColor = m_desc.color;
	std::shared_ptr<CModel> ColumnModel = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, descColumn);
	if (nullptr == ColumnModel) {
		PRINTLOG("Fail to create column model");
		return entt::null;
	}
	entt::entity entityColumn = SceneGraph::GetInstance().CreateModel(*ColumnModel.get());

	CModel::S_MODEL_DESC descArrow;
	descArrow.strName = (std::string("CONE_") + m_strName);
	descArrow.S_MODEL_CONE_DESC.center = descColumn.S_MODEL_COLUMN_DESC.end;
	descArrow.S_MODEL_CONE_DESC.axisDir = (descColumn.S_MODEL_COLUMN_DESC.end - descColumn.S_MODEL_COLUMN_DESC.start);
	descArrow.S_MODEL_CONE_DESC.radius = descColumn.S_MODEL_COLUMN_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.height = descArrow.S_MODEL_CONE_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.color = descColumn.S_MODEL_COLUMN_DESC.sColor;
	std::shared_ptr<CModel> ArrowModel = CModelLoader::LoadModel(CModel::E_MODEL_CONE, descArrow);
	if (nullptr == ArrowModel) {
		PRINTLOG("Fail to create cone model");
		return entt::null;
	}
	entt::entity entityArrow = SceneGraph::GetInstance().CreateModel(*ArrowModel.get());
	SceneGraph::GetInstance().BindModel(entityColumn, entityArrow);
	return entityColumn;
}
