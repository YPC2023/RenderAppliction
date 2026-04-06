#include "CRenderSystem.h"
#include "../Model/CModel.h"
#include <CUtils.h>

bool		CRenderSystem::m_bNeed_Initialize = true;

void CRenderSystem::Initialize(CRenderContext& context, CSceneGraphManager& scene)
{
	SetNeedInitialFlag(true);

	// 旋转最顶层根节点
	(void)GetTopNode(scene);
	
	// 计算拓扑排序
	(void)GetTopologyOrder(scene);

	SetNeedInitialFlag(false);
}

void CRenderSystem::Update(CSceneGraphManager& scene)
{
	// 更新节点的根节点
	UpdateNodeRoot(scene);

	// 更新transform节点
	UpdateTransform(scene);
}

void CRenderSystem::Render(const CRenderContext& context, const CSceneGraphManager& scene)
{
	const auto view = scene.QueryAttributes<CSceneGraphComponent::S_MESH_INFO>();
	for (auto entity : view) {
		const CSceneGraphComponent::S_MESH_INFO& mesh = scene.QueryAttribute<CSceneGraphComponent::S_MESH_INFO>(entity);
		RenderMesh(context, scene, entity);
	}
}

std::vector<entt::entity> CRenderSystem::GetTopNode(const CSceneGraphManager& scene)
{
	static std::vector<entt::entity> vecNodes;
	if (!IsNeedInitialize()) {
		return vecNodes;
	}
	vecNodes.clear();
	auto view = scene.QueryAttributes<CSceneGraphComponent::S_RELATION_INFO>();
	for (auto entity : view) {
		const auto& relation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
		// 只有根节点（没有父节点）才开始 DFS 递归
		if (relation.parent == entt::null) {
			vecNodes.push_back(entity);
		}
	}
	return vecNodes;
}

void CRenderSystem::UpdateNodeRoot(CSceneGraphManager& scene, entt::entity root, entt::entity entity)
{
	// 查询子节点
	auto& relation = scene.QueryAttributeModify<CSceneGraphComponent::S_RELATION_INFO>(entity);
	relation.root = root;

	// 遍历处理子节点
	for (auto child : relation.children)
	{
		UpdateNodeRoot(scene, root, child);
	}
}

void CRenderSystem::UpdateNodeRoot(CSceneGraphManager& scene)
{
	std::vector<entt::entity> vecTops = GetTopNode(scene);
	for (size_t index = 0; index < vecTops.size(); ++index) {
		// 查询子节点
		const auto& relation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(vecTops[index]);
		// 遍历处理子节点
		for (auto child : relation.children)
		{
			UpdateNodeRoot(scene, vecTops[index], child);
		}
	}
}

void CRenderSystem::UpdateTransform(CSceneGraphManager& scene)
{
	// 获取拓扑排序序列
	std::vector<entt::entity> vecNodes = GetTopologyOrder(scene);
	for (entt::entity entity : vecNodes) {
		// 查询节点的“关系”组件
		const CSceneGraphComponent::S_RELATION_INFO& CurrentRelation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
		// 查询节点的“变换”组件
		CSceneGraphComponent::S_TRANSFORM_INFO& CurrentTransform = scene.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(entity);
		glm::mat4 baseMatrix(1.0f);
		if (entt::null != CurrentRelation.parent) {
			const CSceneGraphComponent::S_TRANSFORM_INFO& ParentTransform = scene.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(CurrentRelation.parent);
			baseMatrix = ParentTransform.matrix;
		}
		CurrentTransform.update(baseMatrix);
	}
}

void CRenderSystem::SortTopologyNode(const CSceneGraphManager& scene,
	entt::entity entity, std::vector<entt::entity>& vecNodes)
{
	vecNodes.push_back(entity); // 先存父节点
	// 查询子节点
	const auto& relation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
	// 遍历处理子节点
	for (auto child : relation.children) 
	{
		SortTopologyNode(scene, child, vecNodes);
	}
}

std::vector<entt::entity> CRenderSystem::GetTopologyOrder(const CSceneGraphManager& scene)
{
	static std::vector<entt::entity> vecNodes;
	// 判断用户是否要求重新排序
	if (!IsNeedInitialize()) {
		return vecNodes;
	}
	vecNodes.clear();

	std::vector<entt::entity> vecTops = GetTopNode(scene);
	for (size_t index = 0; index < vecTops.size(); ++index) {
		SortTopologyNode(scene, vecTops[index], vecNodes);
	}
	return vecNodes;
}

void CRenderSystem::RenderMesh(const CRenderContext& context, 
	const CSceneGraphManager& scene, entt::entity entity)
{
	// 默认的材质
	std::shared_ptr<CMaterial> material = context.m_Material;

	// 如果没有默认材质，就查询“伴随”的材质
	if (nullptr == material) {// 获取材质
		const CSceneGraphComponent::S_MATERIAL_INFO LocalMaterial = scene.QueryAttribute<CSceneGraphComponent::S_MATERIAL_INFO>(entity);
		material = LocalMaterial.m_Material;
	}

	material->m_shader->use();
	// 在只有mvp但是没有使用ubo的mvp时，需要本地设置投影矩阵和视图矩阵，否则在session中统一通过ubo设置
	if (material->m_shader->GetDesc().hasMVP && !material->m_shader->GetDesc().hasMVP_UBO) {
		material->m_shader->setMat4("projection", context.m_Camera->GetProjection());
		material->m_shader->setMat4("view", context.m_Camera->GetView());
	}
	
	const CSceneGraphComponent::S_RELATION_INFO& relation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
	// 渲染选中高光
	if (material->GetDesc().hasSelected) {
		material->m_shader->setBool("selectedID", 0 < context.m_set_SelectedId.count(relation.root));
	}

	// 渲染拾取纹理图
	if (context.m_RenderID) {
		SetRenderId(context, material, relation.root);
	}

	const CSceneGraphComponent::S_MESH_INFO& mesh = scene.QueryAttribute<CSceneGraphComponent::S_MESH_INFO>(entity);
	const CSceneGraphComponent::S_TRANSFORM_INFO& Transform = scene.QueryAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(entity);

	material->m_shader->setMat4("model", Transform.matrix);
	// 激活纹理
	ActiveTexture(context, material, mesh);

	// draw mesh
	glBindVertexArray(mesh.VAO);
	glDrawElements(mesh.Type, static_cast<unsigned int>(mesh.size), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void CRenderSystem::ActiveTexture(const CRenderContext& context, 
	std::shared_ptr<CMaterial> material, 
	const CSceneGraphComponent::S_MESH_INFO& mesh)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (size_t index = 0; index < material->m_desc.textures.size(); ++index)
	{
		glActiveTexture((GLenum)(GL_TEXTURE0 + index)); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = material->m_desc.textures[index].strType;
		if (name == "texture_diffuse")
			number = CUtils::UintToString(diffuseNr++);
		else if (name == "texture_specular")
			number = CUtils::UintToString(specularNr++); // transfer unsigned int to string
		else if (name == "texture_normal")
			number = CUtils::UintToString(normalNr++); // transfer unsigned int to string
		else if (name == "texture_height")
			number = CUtils::UintToString(heightNr++); // transfer unsigned int to string
		name += number;
		// now set the sampler to the correct texture unit
		material->m_shader->setInt(name.c_str(), (int)index);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, material->m_desc.textures[index].texture->GetID());
	}
}

void CRenderSystem::SetRenderId(const CRenderContext& context, 
	std::shared_ptr<CMaterial> material, entt::entity entity)
{
	unsigned int ID = (unsigned int)entity;
	float v3 = (float)((ID >> 24) & 0xFF);
	float v2 = (float)((ID >> 16) & 0xFF);
	float v1 = (float)((ID >> 8) & 0xFF);
	float v0 = (float)(ID & 0xFF);
	material->m_shader->setVec4("objectID", glm::vec4(v0 / 255, v1 / 255, v2 / 255, v3 / 255));
}

void CRenderSystem::SetNeedInitialFlag(bool bYes)
{
	m_bNeed_Initialize = bYes;
}

bool CRenderSystem::IsNeedInitialize()
{
	return m_bNeed_Initialize;
}