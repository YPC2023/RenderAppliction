#include "CRenderSystem.h"
#include "../Model/CModel.h"
#include <CUtils.h>


void CRenderSystem::Update(CSceneGraphManager& scene)
{
	const auto view = scene.QueryAttributes<CSceneGraphComponent::S_MODEL_INFO>();
	for (auto entity : view) {
		const CSceneGraphComponent::S_RELATION_INFO& model = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
		CSceneGraphComponent::S_TRANSFORM_INFO& Transform = scene.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(entity);
		Transform.update(glm::mat4(1.0f));
		std::set<entt::entity>::const_iterator it = model.children.begin();
		for (; it != model.children.end(); ++it) {
			UpdateNode(scene, *it);
		}
	}
}

void CRenderSystem::Render(const CRenderContext& context, const CSceneGraphManager& scene)
{
	context.m_Material->m_shader->use();
	context.m_Material->m_shader->setMat4("projection", context.m_Camera->GetProjection());
	context.m_Material->m_shader->setMat4("view", context.m_Camera->GetView());
	
	const auto view = scene.QueryAttributes<CSceneGraphComponent::S_MODEL_INFO>();
	for (auto entity : view) {

		// 渲染选中高光
		context.m_Material->m_shader->setBool("selectedID", entity == context.m_SelectedId);

		// 渲染拾取纹理图
		if (context.m_RenderID) {
			SetRenderId(context, entity);
		}

		const CSceneGraphComponent::S_RELATION_INFO& model = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
		std::set<entt::entity>::const_iterator it = model.children.begin();
		for (; it != model.children.end(); ++it) {
			RenderMesh(context, scene, *it);
		}
	}
}

void CRenderSystem::UpdateNode(CSceneGraphManager& scene, entt::entity entity)
{
	const CSceneGraphComponent::S_RELATION_INFO& CurrentRelation = scene.QueryAttribute<CSceneGraphComponent::S_RELATION_INFO>(entity);
	glm::mat4 baseMatrix(1.0f);
	if (entt::null != CurrentRelation.parent) {
		const CSceneGraphComponent::S_TRANSFORM_INFO& Parent = scene.QueryAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(CurrentRelation.parent);
		baseMatrix = Parent.matrix;
	}
	CSceneGraphComponent::S_TRANSFORM_INFO& CurrentTransform = scene.QueryAttributeModify<CSceneGraphComponent::S_TRANSFORM_INFO>(entity);
	CurrentTransform.update(baseMatrix);
	std::set<entt::entity>::const_iterator it = CurrentRelation.children.begin();
	for (; it != CurrentRelation.children.end(); ++it) {
		UpdateNode(scene, *it);
	}
}

void CRenderSystem::RenderMesh(const CRenderContext& context, const CSceneGraphManager& scene, entt::entity entity)
{
	const CSceneGraphComponent::S_MESH_INFO& mesh = scene.QueryAttribute<CSceneGraphComponent::S_MESH_INFO>(entity);
	const CSceneGraphComponent::S_TRANSFORM_INFO& Transform = scene.QueryAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(entity);

	context.m_Material->m_shader->setMat4("model", Transform.matrix);
	// 激活纹理
	ActiveTexture(context, mesh);

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

void CRenderSystem::ActiveTexture(const CRenderContext& context, const CSceneGraphComponent::S_MESH_INFO& mesh)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (size_t index = 0; index < mesh.textures.size(); ++index)
	{
		glActiveTexture((GLenum)(GL_TEXTURE0 + index)); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = mesh.textures[index].strType;
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
		context.m_Material->m_shader->setInt(name.c_str(), (int)index);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, mesh.textures[index].texture->GetID());
	}
}

void CRenderSystem::SetRenderId(const CRenderContext& context, entt::entity entity)
{
	unsigned int ID = (unsigned int)entity;
	float v3 = (float)((ID >> 24) & 0xFF);
	float v2 = (float)((ID >> 16) & 0xFF);
	float v1 = (float)((ID >> 8) & 0xFF);
	float v0 = (float)(ID & 0xFF);
	context.m_Material->m_shader->setVec4("objectID", glm::vec4(v0 / 255, v1 / 255, v2 / 255, v3 / 255));
}