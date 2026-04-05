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
		E_MODEL_COLUMN,		// 圆柱体
		E_MODEL_SPHERE,		// 球体 
		E_MODEL_CONE,		// 圆锥体
		E_MODEL_TORUS,		// 圆环
	}E_MODEL_TYPE;
	typedef struct _S_MODEL_DESC
	{
		std::string		strPath = "";
		float			vertexResize = 1.0f;
		float			textureResize = 1.0f;
		// 圆柱体配置
		struct _S_MODEL_COLUMN_DESC
		{
			glm::vec3 start = glm::vec3(0.0f);
			glm::vec3 end = glm::vec3(1.0f, 0.0f, 0.0f);
			float radius = 1.0f;
			int sectors = 32;
		}S_MODEL_COLUMN_DESC;
		struct _S_MODEL_SPHERE_DESC
		{
			glm::vec3 center = glm::vec3(0.0f);
			glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			float radius = 1.0f;
			int sectors = 320;
			int stacks = 640;
		}S_MODEL_SPHERE_DESC;
		struct _S_MODEL_CONE_DESC
		{
			glm::vec3 center = glm::vec3(0.0f);
			glm::vec3 axisDir = glm::vec3(0.0f, 0.0f, 1.0f);
			float height = 1.0f;
			float radius = 1.0f;
			glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			int sectors = 32;
		}S_MODEL_CONE_DESC;
		struct _S_MODEL_TORUS_DESC
		{
			glm::vec3 center = glm::vec3(0.0f);
			glm::vec3 axisDir = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			float radius = 5.0f;
			float width = 1.0f;
			int mainSectors = 64;
			int tubeSectors = 32;
		}S_MODEL_TORUS_DESC;
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
	bool InitializeColumn();
	bool InitializeSphere();
	bool InitializeCone();
	bool InitializeTorus();

private:
	static void GenerateTorusVertex(const glm::vec3& center, const glm::vec3& axisDir, const glm::vec4& color,
		float majorRadius, float minorRadius, int mainSectors, int tubeSectors,
		std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices);
	static void GenerateConeVertex(const glm::vec3& center, const glm::vec3& axisDir,
		float height, float radius, int sectors, const glm::vec4& color,
		std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices);
	static void GenerateSphereVertex(const glm::vec3& center, float radius, int sectors, int stacks,
		const glm::vec4& color, std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices);
	static void GenerateColumnVertex(glm::vec3 start, glm::vec3 end,
		std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices, float radius = 1.0f, int sectors = 32,
		const glm::vec4& sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), const glm::vec4& eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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