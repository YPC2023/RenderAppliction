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
	enum class E_MODEL_TYPE
	{
		E_MODEL_CHESS,		// ∆Â≈Ã∏Ò
	};
public:
	CModel(bool bAutoRecycle = true);
	CModel(E_MODEL_TYPE type);
	CModel(const std::string& strPth,bool bAutoRecycle = true);
private:
	bool LoadModel(const std::string& path);
	bool ProcessNode(aiNode* node, const aiScene* scene);
	CMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<CMesh::S_TEXTURE> LoadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
private:
	bool InitializeChess();
private:
	bool					m_ok;
	std::string				m_strRootPath;
	bool					m_bAutoRecycle;
public:
	std::string				m_strPath;
	std::vector<CMesh>		m_vec_mesh;
};