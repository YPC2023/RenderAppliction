#include "CoordinateArc.h"
#include "SceneGraph.h"
#include <CUtils.h>


CoordinateArc::CoordinateArc(const S_COORDS_DESC& desc, const std::string& strName):
	m_desc(desc), m_strName(strName)
{
	m_ModelId = SetupCoordinateArc();
}

CoordinateArc::~CoordinateArc()
{
	std::set<entt::entity> setNode = SceneGraph::GetInstance().GetModelTransformComponents(m_ModelId);
	for (auto entity : setNode) {
		SceneGraph::GetInstance().RemoveNode(entity);
	}
}

entt::entity CoordinateArc::SetupCoordinateArc()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "RINGARC";
	desc.S_MODEL_RINGARC_DESC.center = m_desc.center;
	desc.S_MODEL_RINGARC_DESC.normal = m_desc.normal;
	desc.S_MODEL_RINGARC_DESC.refVector = m_desc.refVector;
	desc.S_MODEL_RINGARC_DESC.sColor = m_desc.sColor;
	desc.S_MODEL_RINGARC_DESC.eColor = m_desc.eColor;
	desc.S_MODEL_RINGARC_DESC.startAngle = m_desc.startAngle;
	desc.S_MODEL_RINGARC_DESC.endAngle = m_desc.endAngle;
	desc.S_MODEL_RINGARC_DESC.outerRadius = m_desc.outerRadius;
	desc.S_MODEL_RINGARC_DESC.ringWidth = m_desc.ringWidth;
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_RINGARC, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return entt::null;
	}
	return SceneGraph::GetInstance().CreateModel(*Model.get());
}