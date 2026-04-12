#include "SceneGraph.h"
#include <CUtils.h>

entt::entity SceneGraph::CreateNode()
{
	entt::entity entity = m_Registry.create();
	ASSERT(entt::null != entity);
	return entity;
}

void SceneGraph::RemoveNode(entt::entity entity)
{
	if (!EntityIsValid(entity)) {
		return;
	}
	m_Registry.destroy(entity);
}

const SGCmpnt::S_CMPNT_MATERIAL& SceneGraph::GetCmpntMaterial(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MATERIAL>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_MATERIAL>(entity);
}

SGCmpnt::S_CMPNT_MATERIAL& SceneGraph::GetCmpntMaterial(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MATERIAL>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_MATERIAL>(entity);
}

const SGCmpnt::S_CMPNT_MESH& SceneGraph::GetCmpntMesh(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MESH>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_MESH>(entity);
}

SGCmpnt::S_CMPNT_MESH& SceneGraph::GetCmpntMesh(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MESH>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_MESH>(entity);
}

const SGCmpnt::S_CMPNT_TRANSFORM_DATA& SceneGraph::GetCmpntTransformData(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_TRANSFORM_DATA>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_TRANSFORM_DATA>(entity);
}

SGCmpnt::S_CMPNT_TRANSFORM_DATA& SceneGraph::GetCmpntTransformData(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_TRANSFORM_DATA>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_TRANSFORM_DATA>(entity);
}

const SGCmpnt::S_CMPNT_MODEL& SceneGraph::GetCmpntModel(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_MODEL>(entity);
}

SGCmpnt::S_CMPNT_MODEL& SceneGraph::GetCmpntModel(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_MODEL>(entity);
}

const SGCmpnt::S_CMPNT_RELATION_MESH& SceneGraph::GetCmpntRelationMesh(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_RELATION_MESH>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_RELATION_MESH>(entity);
}

SGCmpnt::S_CMPNT_RELATION_MESH& SceneGraph::GetCmpntRelationMesh(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_RELATION_MESH>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_RELATION_MESH>(entity);
}

const SGCmpnt::S_CMPNT_RELATION_MODEL& SceneGraph::GetCmpntRelationModel(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_RELATION_MODEL>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_RELATION_MODEL>(entity);
}

SGCmpnt::S_CMPNT_RELATION_MODEL& SceneGraph::GetCmpntRelationModel(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_RELATION_MODEL>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_RELATION_MODEL>(entity);
}

const SGCmpnt::S_CMPNT_RELATION_TRANSFORM& SceneGraph::GetCmpntRelationTransform(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_RELATION_TRANSFORM>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_RELATION_TRANSFORM>(entity);
}

SGCmpnt::S_CMPNT_RELATION_TRANSFORM& SceneGraph::GetCmpntRelationTransform(entt::entity entity)
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_RELATION_TRANSFORM>(entity));
	return QueryComponent<SGCmpnt::S_CMPNT_RELATION_TRANSFORM>(entity);
}

const std::vector<entt::entity> SceneGraph::GetModelTransformComponents(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	std::vector<entt::entity> vecModel;
	// 获取所有Model节点
	GetModelTopologyComponents(entity, vecModel);
	std::vector<entt::entity> vecNode;
	// 遍历Model节点，获取他们的Mesh节点
	for (auto model : vecModel) {
		const auto& Relation = GetCmpntRelationMesh(model);
		// 压入Model节点
		vecNode.push_back(model);
		// 遍历所有mesh节点
		for (auto mesh : Relation.children) {
			vecNode.push_back(mesh);
		}
	}
	// 包含Model和Mesh集
	return vecNode;
}

const std::vector<entt::entity> SceneGraph::GetModelTransformMeshComponents(entt::entity entity) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	std::vector<entt::entity> vecModel;
	// 获取所有Model节点
	GetModelTopologyComponents(entity, vecModel);
	std::vector<entt::entity> vecMesh;
	// 遍历Model节点，获取他们的Mesh节点
	for (auto model : vecModel) {
		const auto& Relation = GetCmpntRelationMesh(model);
		// 遍历所有mesh节点
		for (auto mesh : Relation.children) {
			vecMesh.push_back(mesh);
		}
	}
	// 包含Model和Mesh集
	return vecMesh;
}

const std::vector<entt::entity> SceneGraph::GetModelTopComponentes() const
{
	std::vector<entt::entity> vecNodes;
	auto views = QueryComponentes<SGCmpnt::S_CMPNT_MODEL>();
	for (auto entity : views) {
		const auto& Relation = GetCmpntRelationTransform(entity);
		if (entt::null == Relation.parent) {
			vecNodes.push_back(entity);
		}
	}
	return vecNodes;
}

const void SceneGraph::GetModelTopologyComponents(entt::entity entity, std::vector<entt::entity>& vecNodes) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	vecNodes.push_back(entity);
	const auto& Relation = GetCmpntRelationTransform(entity);
	for (auto child : Relation.children) {
		GetModelTopologyComponents(child, vecNodes);
	}
}

const std::vector<entt::entity> SceneGraph::GetModelTopologyComponents() const
{
	std::vector<entt::entity> vecNodes;
	std::vector<entt::entity> vecTopNodes = GetModelTopComponentes();
	for (auto entity : vecTopNodes) {
		GetModelTopologyComponents(entity, vecNodes);
	}
	return vecNodes;
}

const void SceneGraph::GetMeshTopologyComponents(entt::entity entity, std::vector<entt::entity>& vecNodes) const
{
	ASSERT(HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	const auto& MeshRelation = GetCmpntRelationMesh(entity);
	for (auto child : MeshRelation.children) {
		vecNodes.push_back(child);
	}
	const auto& Relation = GetCmpntRelationTransform(entity);
	for (auto child : Relation.children) {
		GetMeshTopologyComponents(child, vecNodes);
	}
}

const std::vector<entt::entity> SceneGraph::GetMeshTopologyComponents() const
{
	std::vector<entt::entity> vecNodes;
	std::vector<entt::entity> vecTopNodes = GetModelTopComponentes();
	for (auto entity : vecTopNodes) {
		GetMeshTopologyComponents(entity, vecNodes);
	}
	return vecNodes;
}

void SceneGraph::BindModel(entt::entity parent, entt::entity child, bool selected)
{
	BindModelTransform(parent, child);
	if (selected) {
		BindModelSelected(child);
	}
	else {
		UnBindModelSelected(child);
	}
}

void SceneGraph::UnBindModel(entt::entity entity)
{
	UnBindModelTransform(entity);
	UnBindModelSelected(entity);
}

void SceneGraph::BindModelTransform(entt::entity parent, entt::entity child)
{
	auto& ChildTransform = GetCmpntRelationTransform(child);
	if (ChildTransform.parent == parent) {
		return;
	}
	if (entt::null != ChildTransform.parent) {
		UnBindModelTransform(child);
	}
	auto& ParentTransform = GetCmpntRelationTransform(parent);
	ParentTransform.children.insert(child);
	ChildTransform.parent = parent;
}

void SceneGraph::UnBindModelTransform(entt::entity entity)
{
	auto& CurTransform = GetCmpntRelationTransform(entity);
	if (entt::null != CurTransform.parent) {
		auto& ParentTransform = GetCmpntRelationTransform(CurTransform.parent);
		ParentTransform.children.erase(entity);
	}
	CurTransform.parent = entt::null;
}

void SceneGraph::BindModelSelected(entt::entity entity)
{
	auto& Transform = GetCmpntRelationTransform(entity);
	Transform.selected_with_parent = true;
}

void SceneGraph::UnBindModelSelected(entt::entity entity)
{
	auto& Transform = GetCmpntRelationTransform(entity);
	Transform.selected_with_parent = false;
}

std::string SceneGraph::GenPointString(entt::entity parent, entt::entity child) const
{
	std::string strDetail = CUtils::UintToString((unsigned int)parent);
	strDetail += " -> ";
	strDetail += CUtils::UintToString((unsigned int)child);
	return strDetail;
}

std::string SceneGraph::GetComponentDetail(entt::entity entity) const
{
	std::string strDetail = "";

	return strDetail;
}

void SceneGraph::GetRelationDetail(std::string& strDetail, entt::entity entity) const
{
	const auto& Meshes = GetCmpntRelationMesh(entity);
	for (auto child : Meshes.children) {
		strDetail += "\n";
		strDetail += GenPointString(entity, child);
	}
	const auto& Relation = GetCmpntRelationTransform(entity);
	for (auto child : Relation.children) {
		GetRelationDetail(strDetail, child);
	}
	strDetail += "\n";
	strDetail += GenPointString(Relation.parent, entity);
}

std::string SceneGraph::GetRelationDetail() const
{
	std::string strDetail = "";

	std::vector<entt::entity> vecNodes = GetModelTopComponentes();
	for (auto entity : vecNodes) {
		GetRelationDetail(strDetail, entity);
	}

	return strDetail;
}

entt::entity SceneGraph::CreateModel(const CModel& model)
{
	entt::entity entity = CreateNode();
	
	(void)AppendComponent<SGCmpnt::S_CMPNT_MODEL>(entity, model.m_strName);
	(void)AppendComponent<SGCmpnt::S_CMPNT_RELATION_MESH>(entity);
	(void)AppendComponent<SGCmpnt::S_CMPNT_TRANSFORM_DATA>(entity);
	(void)AppendComponent<SGCmpnt::S_CMPNT_RELATION_TRANSFORM>(entity);
	
	for (size_t index = 0; index < model.m_vec_mesh.size(); ++index) {
		(void)CreateMesh(entity, model.m_vec_mesh[index]);
	}

	return entity;
}

entt::entity SceneGraph::CreateMesh(const entt::entity model, const CMesh& mesh)
{
	entt::entity entity = CreateNode();

	auto& MeshDetail = AppendComponent<SGCmpnt::S_CMPNT_MESH>(entity);
	MeshDetail.Type = mesh.m_nType;
	MeshDetail.VAO = mesh.m_VAO;
	MeshDetail.VBO = mesh.m_VBO;
	MeshDetail.EBO = mesh.m_EBO;
	MeshDetail.size = mesh.m_vec_Indices.size();

	(void)AppendComponent<SGCmpnt::S_CMPNT_TRANSFORM_DATA>(entity);
	// model和mesh互相指向
	auto& ParentRelation = GetCmpntRelationMesh(model);
	auto& ChildRelation = AppendComponent<SGCmpnt::S_CMPNT_RELATION_MODEL>(entity);
	ParentRelation.children.insert(entity);
	ChildRelation.parent = model;

	// 添加材质节点
	auto& Material = AppendComponent<SGCmpnt::S_CMPNT_MATERIAL>(entity);
	Material.m_Material = CMaterialSystem::AquireMaterial(CModelLoader::TranslateMaterial(mesh));
	(void)AppendComponent<SGCmpnt::S_CMPNT_RELATION_TRANSFORM>(entity);
	
	return entity;
}
