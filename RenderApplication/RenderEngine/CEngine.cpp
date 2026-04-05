#include "CEngine.h"
#include "Session/CSessionManager.h"
#include "Render/CRenderSystem.h"
#include "Model/CModelLoader.h"
#include <CUtils.h>

bool CEngine::Initialize()
{
	return true;
}

void CEngine::UnInitialize()
{
}

bool CEngine::SetLoader(GLADloadproc loader)
{
	loader = loader;
	return true;
}

void CEngine::Render()
{
	
}

unsigned int CEngine::GetRenderTextureId()
{
	return 0;
}


void CEngine::Resize(int width, int height)
{

}


bool CEngine::LoadModel(const char* path)
{
	// 从文件加载模型文件
	std::shared_ptr<CModel> FileModel = CModelLoader::LoadFileModel(path, false);
	if (nullptr == FileModel) {
		PRINTLOG("Fail to load Model(%s)", path);
		return false;
	}
	// 创建model节点
	entt::entity entityModel = CSceneGraphManager::GetInstance().CreateNode();
	if (entt::null == entityModel) {
		PRINTLOG("Fail to create scene model node");
		return false;
	}
	// 添加model节点并返回引用
	CSceneGraphManager::GetInstance().AppendAttribute<CSceneGraphComponent::S_MODEL_INFO>(entityModel, FileModel->m_strPath);
	// 给model添加transform组件
	CSceneGraphManager::GetInstance(). AppendAttribute<CSceneGraphComponent::S_TRANSFORM_INFO>(entityModel);
	// 给model添加relation组件
	CSceneGraphComponent::S_RELATION_INFO& ModelRelation = CSceneGraphManager::GetInstance().
		AppendAttribute<CSceneGraphComponent::S_RELATION_INFO>(entityModel);

	for (size_t indexMesh = 0; indexMesh < FileModel->m_vec_mesh.size(); ++indexMesh) {
		// 创建mesh节点
		entt::entity entityMesh = CSceneGraphManager::GetInstance().CreateNode();
		if (entt::null == entityMesh) {
			PRINTLOG("Fail to create scene mesh node");
			return false;
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

		mesh.Type = FileModel->m_vec_mesh[indexMesh].m_nType;
		mesh.VAO = FileModel->m_vec_mesh[indexMesh].m_VAO;
		mesh.VBO = FileModel->m_vec_mesh[indexMesh].m_VBO;
		mesh.EBO = FileModel->m_vec_mesh[indexMesh].m_EBO;
		mesh.size = FileModel->m_vec_mesh[indexMesh].m_vec_Indices.size();

		for (size_t indexTexture = 0; 
			indexTexture < FileModel->m_vec_mesh[indexMesh].m_vec_Textures.size(); ++indexTexture) {
			CSceneGraphComponent::S_TEXTURE_INFO texture;
			texture.strName = FileModel->m_vec_mesh[indexMesh].m_vec_Textures[indexTexture].strPath;
			texture.strType = FileModel->m_vec_mesh[indexMesh].m_vec_Textures[indexTexture].strType;
			texture.texture = FileModel->m_vec_mesh[indexMesh].m_vec_Textures[indexTexture].texture;
			mesh.textures.push_back(texture);
		}
	}
	return true;
}

void CEngine::OnMouseAction(E_MOUSE_BUTTON_TYPE key, E_MOUSE_ACTION_TYPE action, int x, int y)
{

}