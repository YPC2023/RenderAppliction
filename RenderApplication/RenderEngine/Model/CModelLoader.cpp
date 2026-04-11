#include "CModelLoader.h"
#include <CUtils.h>

std::shared_ptr<CModel> CModelLoader::LoadModel(CModel::E_MODEL_TYPE type, const CModel::S_MODEL_DESC& desc)
{
	std::shared_ptr<CModel> model = std::make_shared<CModel>(type, desc);
	if (nullptr == model || !model->m_ok) {
		PRINTLOG("Fail to load model");
		return nullptr;
	}
	return model;
}

CMaterial::S_MATERIAL_DESC CModelLoader::TranslateMaterial(const CMesh& mesh)
{
	CMaterial::S_MATERIAL_DESC desc;

	if (0 < mesh.m_vec_Vertices.size()) {
		const CMesh::S_VERTEX& vertex = mesh.m_vec_Vertices[0];
		
		desc.hasPosition = vertex.Position.m_enable;
		desc.hasColor = vertex.Color.m_enable;
		desc.hasNormal = vertex.Normal.m_enable;
		desc.hasTexCoords = vertex.TexCoords.m_enable;
		desc.hasTangent = vertex.Tangent.m_enable;
		desc.hasBitangent = vertex.Bitangent.m_enable;
	}
	for (size_t index = 0; index < mesh.m_vec_Textures.size(); ++index) {
		desc.textures.push_back({
			mesh.m_vec_Textures[index].strType,
			mesh.m_vec_Textures[index].texture
			});
	}
	return desc;
}