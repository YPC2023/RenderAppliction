#include "Graphviz.h"
#include <CUtils.h>


void Graphviz::RenderGraph(const SceneGraph& scene, const std::string& strPath)
{
	S_CONTEXT cxt(scene);
	cxt.gvc = gvContext();
	cxt.g = agopen((char*)"SceneGraph", Agdirected, NULL);

	m_map_Node.clear();
	RenderModel(cxt);
	RenderMesh(cxt);

	// 执行布局
	gvLayout(cxt.gvc, cxt.g, (char*)"dot");

	// 渲染为 SVG 格式（最适合网页展示）
	gvRenderFilename(cxt.gvc, cxt.g, (char*)"svg", (char*)strPath.c_str());
	gvFreeLayout(cxt.gvc, cxt.g);
	agclose(cxt.g);
	gvFreeContext(cxt.gvc);
}

std::string Graphviz::GetModelContent(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return "";
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	std::string strContent = "MODEL[";
	strContent += CUtils::UintToString((unsigned int)entity);
	strContent += "]\n";

	const auto& Relation = cxt.scene.GetCmpntRelationTransform(entity);
	strContent += "SelectId[";
	strContent += CUtils::UintToString((unsigned int)Relation.selected_id);
	strContent += "]\n";

	strContent += "TransformId[";
	strContent += CUtils::UintToString((unsigned int)Relation.transform_id);
	strContent += "]";

	return strContent;
}

std::string Graphviz::GetMeshContent(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return "";
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MESH>(entity));

	std::string strContent = "MESH[";
	strContent += CUtils::UintToString((unsigned int)entity);
	strContent += "]\n";

	const auto& Relation = cxt.scene.GetCmpntRelationTransform(entity);
	strContent += "SelectId[";
	strContent += CUtils::UintToString((unsigned int)Relation.selected_id);
	strContent += "]\n";

	strContent += "TransformId[";
	strContent += CUtils::UintToString((unsigned int)Relation.transform_id);
	strContent += "]";
	return strContent;
}

void Graphviz::SetModelNodeStyle(Agnode_t* node)
{
	if (nullptr != node) {
		// 1. 设置样式为 filled
		agsafeset(node, (char*)"style", "filled", "");
		// 2. 设置填充颜色为 lightblue (支持颜色名或 Hex 值)
		agsafeset(node, (char*)"fillcolor", "lightblue", "");
		// 3. (可选) 设置边框颜色
		agsafeset(node, (char*)"color", "blue", "");
		// 4. 设置节点形状为方框
		agsafeset(node, (char*)"shape", "box", "");
	}
}

void Graphviz::SetMeshNodeStyle(Agnode_t* node)
{
	if (nullptr != node) {
		// 1. 设置样式为 filled
		agsafeset(node, (char*)"style", "filled", "");
		// 2. 设置填充颜色为 lightblue (支持颜色名或 Hex 值)
		agsafeset(node, (char*)"fillcolor", "lightgray", "");
		// 3. (可选) 设置边框颜色
		agsafeset(node, (char*)"color", "gray", "");
	}
}

void Graphviz::CreateModelNode(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return;
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	Agnode_t* n = agnode(cxt.g, (char*)CUtils::UintToString((unsigned int)entity).c_str(), 1);
	agsafeset(n, (char*)"label", (char*)GetModelContent(cxt, entity).c_str(), (char*)"");
	SetModelNodeStyle(n);
	m_map_Node[entity] = n;
}

void Graphviz::CreateMeshNode(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return;
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MESH>(entity));
	Agnode_t* n = agnode(cxt.g, (char*)CUtils::UintToString((unsigned int)entity).c_str(), 1);
	agsafeset(n, (char*)"label", (char*)GetMeshContent(cxt, entity).c_str(), (char*)"");
	SetMeshNodeStyle(n);
	m_map_Node[entity] = n;
}

void Graphviz::LinkModelModel(const S_CONTEXT& cxt, entt::entity parent, entt::entity child)
{
	if (entt::null == parent || entt::null == child) {
		return;
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(parent));
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(child));

	Agnode_t* parentNode = m_map_Node[parent];
	Agnode_t* childNode = m_map_Node[child];

	agedge(cxt.g, parentNode, childNode, NULL, 1);
}

void Graphviz::LinkModelMesh(const S_CONTEXT& cxt, entt::entity parent, entt::entity child)
{
	if (entt::null == parent || entt::null == child) {
		return;
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(parent));
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MESH>(child));

	Agnode_t* parentNode = m_map_Node[parent];
	Agnode_t* childNode = m_map_Node[child];

	agedge(cxt.g, parentNode, childNode, NULL, 1);
}

void Graphviz::RenderModel(const S_CONTEXT& cxt)
{
	const auto& view = cxt.scene.GetModelTopologyComponents();
	for (auto& entity : view) {
		CreateModelNode(cxt, entity);
		const auto& Relation = cxt.scene.GetCmpntRelationTransform(entity);
		LinkModelModel(cxt, Relation.parent, entity);
	}
}

void Graphviz::RenderMesh(const S_CONTEXT& cxt)
{
	const auto& view = cxt.scene.GetMeshTopologyComponents();
	for (auto& entity : view) {
		CreateMeshNode(cxt, entity);
		const auto& Relation = cxt.scene.GetCmpntRelationModel(entity);
		LinkModelMesh(cxt, Relation.parent, entity);
	}
}
