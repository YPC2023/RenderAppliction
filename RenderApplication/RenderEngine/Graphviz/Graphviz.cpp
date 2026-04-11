#include "Graphviz.h"
#include <CUtils.h>
#include <inja/inja.hpp>

#define IDM_INJA_TEMPLATE_MODEL_NAME	"template_model.inja"
#define IDM_INJA_TEMPLATE_MESH_NAME		"template_mesh.inja"

void Graphviz::RenderGraph(const SceneGraph& scene, const std::string& strPath)
{
	S_CONTEXT cxt(scene);
	cxt.gvc = gvContext();
	cxt.g = agopen((char*)"SceneGraph", Agdirected, NULL);

	m_map_Node.clear();
	RenderModel(cxt);
	RenderMesh(cxt);

	//RenderToDot(cxt, strPath);
	RenderToSvg(cxt, strPath);
	
	gvFreeLayout(cxt.gvc, cxt.g);
	agclose(cxt.g);
	gvFreeContext(cxt.gvc);
}

void Graphviz::RenderToDot(const S_CONTEXT& cxt, const std::string& strPath)
{
	// 执行布局
	gvLayout(cxt.gvc, cxt.g, (char*)"dot");
	// 渲染为 SVG 格式（最适合网页展示）
	gvRenderFilename(cxt.gvc, cxt.g, (char*)"dot", (char*)strPath.c_str());
}

void Graphviz::RenderToSvg(const S_CONTEXT& cxt, const std::string& strPath)
{
	// 执行布局
	gvLayout(cxt.gvc, cxt.g, (char*)"dot");
	// 渲染为 SVG 格式（最适合网页展示）
	gvRenderFilename(cxt.gvc, cxt.g, (char*)"svg", (char*)strPath.c_str());
}

std::string Graphviz::RenderContent(const std::string& strPath, const inja::json& data)
{
	if (!CUtils::FileExists(strPath.c_str())) {
		PRINTLOG("Path is not exists(%s)", strPath.c_str());
		return "";
	}
	inja::Environment env;
	try {
		return env.render_file(strPath, data);
	}
	catch (const inja::InjaError& e) {
		// 捕获所有 Inja 相关的错误（基类）
		std::cerr << "渲染失败: " << e.what() << std::endl;

		// 如果需要更精细的信息，可以访问公共成员：
		// e.type: 错误类型 (如 "render_error", "parser_error")
		// e.location: 错误发生的行号和列号
		std::cerr << "位置: " << e.location.line << ":" << e.location.column << std::endl;
		PRINTLOG("Fail render template(%s)\n[%d,%d] %s", strPath.c_str(), e.what(), e.location.line, e.location.column);
	}
	return "";
}

inja::json Graphviz::ModelToInjaJson(const S_CONTEXT& cxt, entt::entity entity)
{
	inja::json json;
	if (entt::null == entity) {
		return "";
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));

	const auto& Relation = cxt.scene.GetCmpntRelationTransform(entity);
	const auto& Model = cxt.scene.GetCmpntModel(entity);

	std::string strName = (Model.strName.empty() ? "MODEL_INFO" : Model.strName);
	strName += "[";
	strName += CUtils::UintToString((unsigned int)entity);
	strName += "]";
	json["name"] = strName;
	json["parent"] = Relation.parent;
	json["transform_id"] = Relation.transform_id;
	json["selected_id"] = Relation.selected_id;
	json["selected_with_parent"] = Relation.selected_with_parent;
	json["children"] = inja::json::array();
	for (auto entity : Relation.children) {
		json["children"].push_back(entity);
	}

	return json;
}

inja::json Graphviz::MeshToInjaJson(const S_CONTEXT& cxt, entt::entity entity)
{
	inja::json json;
	if (entt::null == entity) {
		return "";
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MESH>(entity));

	const auto& Relation = cxt.scene.GetCmpntRelationTransform(entity);

	std::string strName = "MODEL_INFO";
	strName += "[";
	strName += CUtils::UintToString((unsigned int)entity);
	strName += "]";
	json["name"] = strName;
	json["parent"] = Relation.parent;
	json["transform_id"] = Relation.transform_id;
	json["selected_id"] = Relation.selected_id;
	json["selected_with_parent"] = Relation.selected_with_parent;
	json["children"] = inja::json::array();
	for (auto entity : Relation.children) {
		json["children"].push_back(entity);
	}

	return json;
}

std::string Graphviz::GetModelContent(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return "";
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	std::string strPath = CUtils::GetGraphvizTemplatePathFile(IDM_INJA_TEMPLATE_MODEL_NAME);
	inja::json data = ModelToInjaJson(cxt, entity);
	std::string strContent = "";
	strContent += RenderContent(strPath, data);
	strContent += "";
	return strContent;
}

std::string Graphviz::GetMeshContent(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return "";
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MESH>(entity));
	std::string strPath = CUtils::GetGraphvizTemplatePathFile(IDM_INJA_TEMPLATE_MESH_NAME);
	inja::json data = MeshToInjaJson(cxt, entity);
	std::string strContent = "";
	strContent += RenderContent(strPath, data);
	strContent += "";
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
		// 关键设置：将节点形状设为 plaintext
		agsafeset(node, (char*)"shape", "plaintext", "");

		// 辅助设置：确保边距为0，适用于更广泛的情况
		agsafeset(node, (char*)"margin", "0", "");

		// 辅助设置：如有需要，强制尺寸由内容决定
		agsafeset(node, (char*)"fixedsize", "false", "");
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

		// 关键设置：将节点形状设为 plaintext
		agsafeset(node, (char*)"shape", "plaintext", "");

		// 辅助设置：确保边距为0，适用于更广泛的情况
		agsafeset(node, (char*)"margin", "0", "");

		// 辅助设置：如有需要，强制尺寸由内容决定
		agsafeset(node, (char*)"fixedsize", "false", "");
	}
}

void Graphviz::CreateModelNode(const S_CONTEXT& cxt, entt::entity entity)
{
	if (entt::null == entity) {
		return;
	}
	ASSERT(cxt.scene.HaveComponent<SGCmpnt::S_CMPNT_MODEL>(entity));
	Agnode_t* n = agnode(cxt.g, (char*)CUtils::UintToString((unsigned int)entity).c_str(), 1);

	char* html_label = agstrdup_html(cxt.g, GetModelContent(cxt, entity).c_str());

	agsafeset(n, (char*)"label", html_label, "");
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

	char* html_label = agstrdup_html(cxt.g, GetMeshContent(cxt, entity).c_str());
	agsafeset(n, (char*)"label", html_label, "");

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
