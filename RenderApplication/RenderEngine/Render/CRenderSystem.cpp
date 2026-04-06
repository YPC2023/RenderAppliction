#include "CRenderSystem.h"
#include "../Model/CModel.h"
#include <CUtils.h>


void CRenderSystem::Update(CSceneGraphManager& scene)
{
	// 获取拓扑排序序列
	std::vector<entt::entity> vecNodes = GetTopologyOrder(scene, true);
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

void CRenderSystem::Render(const CRenderContext& context, const CSceneGraphManager& scene)
{
	const auto view = scene.QueryAttributes<CSceneGraphComponent::S_MODEL_INFO>();
	for (auto entity : view) {
		const CSceneGraphComponent::S_RELATION_INFO& model = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
		std::set<entt::entity>::const_iterator it = model.children.begin();
		for (; it != model.children.end(); ++it) {
			RenderMesh(context, scene, *it);
		}
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

std::vector<entt::entity> CRenderSystem::GetTopologyOrder(const CSceneGraphManager& scene, bool bReSort)
{
	static std::vector<entt::entity> vecNodes;
	// 判断用户是否要求重新排序
	if (!bReSort) {
		return vecNodes;
	}
	auto view = scene.QueryAttributes<CSceneGraphComponent::S_RELATION_INFO>();
	for (auto entity : view) {
		const auto& relation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
		// 只有根节点（没有父节点）才开始 DFS 递归
		if (relation.parent == entt::null) {
			SortTopologyNode(scene, entity, vecNodes);
		}
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
	
	// 渲染选中高光
	if (material->GetDesc().hasSelected) {
		material->m_shader->setBool("selectedID", entity == context.m_SelectedId);
	}

	// 渲染拾取纹理图
	if (context.m_RenderID) {
		SetRenderId(context, material, entity);
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


	// 递归子节点
	CSceneGraphComponent::S_RELATION_INFO Relation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
	std::set<entt::entity>::const_iterator it = Relation.children.begin();
	for (; it != Relation.children.end(); ++it) {
		RenderMesh(context, scene, *it);
	}
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