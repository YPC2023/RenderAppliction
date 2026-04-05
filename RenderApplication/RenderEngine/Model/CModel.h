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
	typedef enum _E_MODEL_TYPE
	{
		E_MODEL_FILE,		// 文件模型
		E_MODEL_CHESS,		// 棋盘格
	}E_MODEL_TYPE;
	typedef struct _S_MODEL_DESC
	{
		std::string		strPath;
		float			vertexResize = 1.0f;
		float			textureResize = 1.0f;
	}S_MODEL_DESC;
public:
	CModel(E_MODEL_TYPE type, const S_MODEL_DESC& desc);
private:
	bool LoadModel(const std::string& path);
	bool ProcessNode(aiNode* node, const aiScene* scene);
	CMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<CMesh::S_TEXTURE> LoadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
private:
	bool InitializeRectangle();
	bool InitializeChess();
private:
	bool					m_ok;
	S_MODEL_DESC			m_desc;
private:
	std::string				m_strRootPath;
	bool					m_bAutoRecycle;
public:
	std::string				m_strPath;
	std::vector<CMesh>		m_vec_mesh;
};