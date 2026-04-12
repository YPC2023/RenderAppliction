#include "CRenderSystem.h"
#include "../Model/CModel.h"
#include <CUtils.h>

void CRenderSystem::Update(const CRenderContext& context, SceneGraph& scene)
{
	// 更新model层节点
	UpdateModel(context, scene);

	// 更新mesh层节点
	UpdateMesh(context, scene);

	//std::string strDetail = scene.GetRelationDetail();
	//PRINTLOG("%s", strDetail.c_str());
}

void CRenderSystem::Render(const CRenderContext& context, const SceneGraph& scene)
{
	const auto view = scene.QueryComponentes<SGCmpnt::S_CMPNT_MESH>();
	for (auto entity : view) {
		// 当前渲染(Session)不允许渲染的Mesh跳过
		if (0 < context.m_set_Unvisible.count(entity)) {
			continue;
		}
		RenderMesh(context, scene, entity);
	}
}

void CRenderSystem::UpdateModel(const CRenderContext& context, SceneGraph& scene)
{
	const std::vector<entt::entity> vecModel = scene.GetModelTopologyComponents();
	for (auto entity : vecModel) {
		const entt::entity camera_id = context.m_Camera->GetModelId();
		// 查询节点的“关系”组件
		auto& CurrentRelation = scene.GetCmpntRelationTransform(entity);
		// 查询节点的“变换”组件
		auto& CurrentTransform = scene.GetCmpntTransformData(entity);
		glm::mat4 baseMatrix(1.0f);
		if (entt::null != CurrentRelation.parent) {
			const auto& ParentTransform = scene.GetCmpntTransformData(CurrentRelation.parent);
			baseMatrix = ParentTransform.matrix.get();
		}
		// 更新变换矩阵
		CurrentTransform.matrix.set(glm::translate(baseMatrix, CurrentTransform.translation.get()));
		CurrentTransform.matrix.set(CurrentTransform.matrix.get() * glm::mat4_cast(CurrentTransform.rotation.get()));
		CurrentTransform.matrix.set(glm::scale(CurrentTransform.matrix.get(), CurrentTransform.scale.get()));

		// 更新选中的ID
		if (CurrentRelation.selected_with_parent && entt::null != CurrentRelation.parent) {
			const auto& ParentRelation = scene.GetCmpntRelationTransform(CurrentRelation.parent);
			CurrentRelation.selected_id = ParentRelation.selected_id;
		}
		else {
			CurrentRelation.selected_id = entity;
		}

		// 更新变换ID
		if (entt::null != CurrentRelation.parent) {
			const auto& ParentRelation = scene.GetCmpntRelationTransform(CurrentRelation.parent);
			CurrentRelation.transform_id = ParentRelation.transform_id;
		}
		else {
			CurrentRelation.transform_id = entity;
		}
	}
}

void CRenderSystem::UpdateMesh(const CRenderContext& context, SceneGraph& scene)
{
	const std::vector<entt::entity> vecModel = scene.GetMeshTopologyComponents();
	for (auto entity : vecModel) {
		// 查询节点的“关系”组件
		auto& CurrentRelation = scene.GetCmpntRelationModel(entity);
		// 查询节点的“变换”组件
		auto& CurrentTransform = scene.GetCmpntTransformData(entity);
		glm::mat4 baseMatrix(1.0f);
		if (entt::null != CurrentRelation.parent) {
			const auto& ParentTransform = scene.GetCmpntTransformData(CurrentRelation.parent);
			baseMatrix = ParentTransform.matrix.get();
		}
		// 更新变换矩阵
		CurrentTransform.matrix.set(glm::translate(baseMatrix, CurrentTransform.translation.get()));
		CurrentTransform.matrix.set(CurrentTransform.matrix.get()* glm::mat4_cast(CurrentTransform.rotation.get()));
		CurrentTransform.matrix.set(glm::scale(CurrentTransform.matrix.get(), CurrentTransform.scale.get()));

		// 更新选中的ID
		auto& CurrentRelationTransform = scene.GetCmpntRelationTransform(entity);
		const auto& ParentRelation = scene.GetCmpntRelationTransform(CurrentRelation.parent);
		CurrentRelationTransform.selected_id = ParentRelation.selected_id;

		// 更新变换ID
		CurrentRelationTransform.transform_id = ParentRelation.transform_id;
	}
}

void CRenderSystem::RenderMesh(const CRenderContext& context, 
	const SceneGraph& scene, entt::entity entity)
{
	// 默认的材质
	std::shared_ptr<CMaterial> material = context.m_Material;

	// 如果没有默认材质，就查询“伴随”的材质
	if (nullptr == material) {// 获取材质
		const auto& LocalMaterial = scene.GetCmpntMaterial(entity);
		material = LocalMaterial.m_Material;
	}

	material->m_shader->use();
	// 在只有mvp但是没有使用ubo的mvp时，需要本地设置投影矩阵和视图矩阵，否则在session中统一通过ubo设置
	if (material->m_shader->GetDesc().hasMVP && !material->m_shader->GetDesc().hasMVP_UBO) {
		material->m_shader->setMat4("projection", context.m_Camera->GetProjection());
		material->m_shader->setMat4("view", context.m_Camera->GetView());
	}
	
	const auto& relation = scene.GetCmpntRelationTransform(entity);
	// 渲染选中高光
	if (material->GetDesc().hasSelected) {
		material->m_shader->setBool("selectedID", 0 < context.m_set_SelectedId.count(relation.selected_id));
	}

	// 渲染拾取纹理图
	if (context.m_RenderID) {
		SetRenderId(context, material, relation.selected_id);
	}

	const auto& mesh = scene.GetCmpntMesh(entity);
	const auto& Transform = scene.GetCmpntTransformData(entity);

	material->m_shader->setMat4("model", Transform.matrix.get());
	//PrintMat4(Transform.matrix.get());
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
	std::shared_ptr<CMaterial> material, const SGCmpnt::S_CMPNT_MESH& mesh)
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

std::string CRenderSystem::FormatVec3(const glm::vec3& value)
{
	std::string strValue = "";
	strValue += CUtils::FloatToString(value.x);
	strValue += ",";
	strValue += CUtils::FloatToString(value.y);
	strValue += ",";
	strValue += CUtils::FloatToString(value.z);
	return strValue;
}

std::string CRenderSystem::FormatVec4(const glm::vec4& value)
{
	std::string strValue = "";
	strValue += CUtils::FloatToString(value.x);
	strValue += ",";
	strValue += CUtils::FloatToString(value.y);
	strValue += ",";
	strValue += CUtils::FloatToString(value.z);
	strValue += ",";
	strValue += CUtils::FloatToString(value.w);
	return strValue;
}

std::string CRenderSystem::FormatMat4(const glm::mat4& value)
{
	std::string strValue = "";
	strValue += FormatVec4(value[0]);
	strValue += "\n";
	strValue += FormatVec4(value[1]);
	strValue += "\n";
	strValue += FormatVec4(value[2]);
	strValue += "\n";
	strValue += FormatVec4(value[3]);
	return strValue;
}

void CRenderSystem::PrintMat4(const glm::mat4& value)
{
	PRINTLOG("%s", FormatMat4(value).c_str());
}
