#include "CEngine.h"
#include "Session/CSessionManager.h"
#include "Render/CRenderSystem.h"
#include "Model/CModelLoader.h"
#include <CUtils.h>

bool CEngine::Initialize()
{
	if (!LoadModelChess()) {
		PRINTLOG("Fail to load chess model");
		return false;
	}
	return true;
}

void CEngine::UnInitialize()
{
}

bool CEngine::SetLoader(GLADloadproc loader)
{
	if (0 == loader) {
		PRINTLOG("Set glfwGetProcAddress first please");
		return false;
	}
	if (!gladLoadGLLoader((GLADloadproc)loader))
	{
		PRINTLOG("Failed to initialize GLAD");
		return false;
	}
	return true;
}


bool CEngine::LoadModel(const char* path)
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strPath = path;
	std::shared_ptr<CModel> FileModel = CModelLoader::LoadModel(CModel::E_MODEL_FILE, desc);
	if (nullptr == FileModel) {
		PRINTLOG("Fail to load Model(%s)", path);
		return false;
	}

	AppendModel(*FileModel.get());
	return true;
}

void CEngine::AppendModel(const CModel& model)
{
	// 创建model节点
	entt::entity entityModel = CSceneGraphManager::GetInstance().CreateNode();
	if (entt::null == entityModel) {
		PRINTLOG("Fail to create scene model node");
		return;
	}
	// 添加model节点并返回引用
	CSceneGraphManager::GetInstance().AppendAttribute<CSceneGraphComponent::S_MODEL_INFO>(entityModel, model.m_strPath);
	// 给model添加transform组件
	CSceneGraphManager::GetInstance().AppendAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(entityModel);
	// 给model添加relation组件
	CSceneGraphComponent::S_RELATION_INFO& ModelRelation = CSceneGraphManager::GetInstance().
		AppendAttribute<CSceneGraphComponent::S_RELATION_INFO>(entityModel);

	for (size_t indexMesh = 0; indexMesh < model.m_vec_mesh.size(); ++indexMesh) {
		// 创建mesh节点
		entt::entity entityMesh = CSceneGraphManager::GetInstance().CreateNode();
		if (entt::null == entityMesh) {
			PRINTLOG("Fail to create scene mesh node");
			return ;
		}
		// 指向子节点
		ModelRelation.children.insert(entityMesh);

		// 获取mesh节点的引用
		CSceneGraphComponent::S_MESH_INFO& mesh = CSceneGraphManager::GetInstance().
			AppendAttribute< CSceneGraphComponent::S_MESH_INFO>(entityMesh);
		// 给mesh添加transform组件
		CSceneGraphManager::GetInstance().AppendAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(entityMesh);
		// 给mesh添加relation组件
		CSceneGraphComponent::S_RELATION_INFO& ModelRelation = CSceneGraphManager::GetInstance().
			AppendAttribute<CSceneGraphComponent::S_RELATION_INFO>(entityMesh);
		// 指向父节点
		ModelRelation.parent = entityModel;

		mesh.Type = model.m_vec_mesh[indexMesh].m_nType;
		mesh.VAO = model.m_vec_mesh[indexMesh].m_VAO;
		mesh.VBO = model.m_vec_mesh[indexMesh].m_VBO;
		mesh.EBO = model.m_vec_mesh[indexMesh].m_EBO;
		mesh.size = model.m_vec_mesh[indexMesh].m_vec_Indices.size();

		for (size_t indexTexture = 0;
			indexTexture < model.m_vec_mesh[indexMesh].m_vec_Textures.size(); ++indexTexture) {
			CSceneGraphComponent::S_TEXTURE_INFO texture;
			texture.strName = model.m_vec_mesh[indexMesh].m_vec_Textures[indexTexture].strPath;
			texture.strType = model.m_vec_mesh[indexMesh].m_vec_Textures[indexTexture].strType;
			texture.texture = model.m_vec_mesh[indexMesh].m_vec_Textures[indexTexture].texture;
			mesh.textures.push_back(texture);
		}
	}
}

bool CEngine::LoadModelChess()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.vertexResize = 10.0f;
	desc.textureResize = 20.0f;
	std::shared_ptr<CModel> FileModel = CModelLoader::LoadModel(CModel::E_MODEL_CHESS, desc);
	if (nullptr == FileModel) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	AppendModel(*FileModel.get());
	return true;
}
