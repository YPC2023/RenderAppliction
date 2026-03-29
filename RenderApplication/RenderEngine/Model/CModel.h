#pragma once

#include <string>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "CMesh.h"
#include "../Resource/CResourceManager.h"

class CModel
{
public:
	CModel(bool bAutoRecycle = true);
	CModel(const std::string& strPth,bool bAutoRecycle = true);
private:
	bool LoadModel(const std::string& path);
	bool ProcessNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<CMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<CMesh::S_TEXTURE> LoadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);

private:
	bool					m_ok;
	std::string				m_strRootPath;
	bool					m_bAutoRecycle;
public:
	std::string							m_strPath;
	std::vector<std::shared_ptr<CMesh>>	m_vec_mesh;
};