#include "CEngine.h"
#include "Session/CSessionManager.h"
#include "Render/CRenderSystem.h"
#include "Model/CModelLoader.h"
#include <CUtils.h>

CEngine* CEngine::m_pInstance = nullptr;

CEngine::CEngine()
{
	
}

CEngine::~CEngine()
{
	SceneGraph::GetInstance().Clear();
}

CEngine& CEngine::GetInstance()
{
	if (nullptr == m_pInstance) {
		m_pInstance = new CEngine();
	}
	return *m_pInstance;
}

void CEngine::ReleaseIntance()
{
	if (nullptr != m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

bool CEngine::Initialize()
{
	/*
	if (!CreateCoordinateAxes()) {
		PRINTLOG("Fail to create CoordinateAxes");
		return false;
	} 
	
	if (!CreateModelChess()) {
		PRINTLOG("Fail to load chess model");
		return false;
	}
	if (!CreateModelColumn()) {
		PRINTLOG("Fail to create column model");
		return false;
	}
	if (!CreateModelSphere()) {
		PRINTLOG("Fail to create sphere model");
		return false;
	}
	
	if (!CreateModelCone()) {
		PRINTLOG("Fail to create cone model");
		return false;
	}
	if (!CreateModelTorus()) {
		PRINTLOG("Fail to create torus model");
		return false;
	}
	
	if (!CreateModelRingArc()) {
		PRINTLOG("Fail to create ring arc model");
		return false;
	}
	*/
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
	desc.strName = CUtils::GetPathFileName(path);
	desc.strPath = path;
	std::shared_ptr<CModel> FileModel = CModelLoader::LoadModel(CModel::E_MODEL_FILE, desc);
	if (nullptr == FileModel) {
		PRINTLOG("Fail to load Model(%s)", path);
		return false;
	}
	
	(void)SceneGraph::GetInstance().CreateModel(*FileModel.get());
	return true;
}

bool CEngine::MergeModel(const std::set<entt::entity>& setModel)
{
	if (2 > setModel.size()) {
		PRINTLOG("Merge object too less(%u)", setModel.size());
		return false;
	}
	entt::entity root = *setModel.begin();
	for (auto it = std::next(setModel.begin()); it != setModel.end(); ++it) {
		SceneGraph::GetInstance().BindModel(root, *it);
	}
	return true;
}

bool CEngine::CreateModelChess()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "CHESS";
	desc.vertexResize = 10.0f;
	desc.textureResize = 20.0f;
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_CHESS, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	(void)SceneGraph::GetInstance().CreateModel(*Model.get());
	return true;
}

bool CEngine::CreateModelColumn()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "COLUMN";
	desc.S_MODEL_COLUMN_DESC.start = glm::vec3(0.0f);
	desc.S_MODEL_COLUMN_DESC.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	desc.S_MODEL_COLUMN_DESC.length = 5.0f;
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	(void)SceneGraph::GetInstance().CreateModel(*Model.get());
	return true;
}

bool CEngine::CreateModelSphere()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "SPHERE";
	desc.S_MODEL_SPHERE_DESC.center = glm::vec3(0.0f);
	desc.S_MODEL_SPHERE_DESC.radius = 4.0f;
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_SPHERE, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	(void)SceneGraph::GetInstance().CreateModel(*Model.get());
	return true;
}

bool CEngine::CreateModelCone()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "CONE";
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_CONE, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	(void)SceneGraph::GetInstance().CreateModel(*Model.get());
	return true;
}

bool CEngine::CreateModelTorus()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "TORUS";
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_TORUS, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	(void)SceneGraph::GetInstance().CreateModel(*Model.get());
	return true;
}

bool CEngine::CreateModelRingArc()
{
	// 从文件加载模型文件
	CModel::S_MODEL_DESC desc;
	desc.strName = "RINGARC";
	std::shared_ptr<CModel> Model = CModelLoader::LoadModel(CModel::E_MODEL_RINGARC, desc);
	if (nullptr == Model) {
		PRINTLOG("Fail to load Model");
		return false;
	}
	(void)SceneGraph::GetInstance().CreateModel(*Model.get());
	return true;
}

bool CEngine::CreateCoordinateAxes()
{
	entt::entity x = CreateCoordinateAxesX();
	entt::entity y = CreateCoordinateAxesY();
	SceneGraph::GetInstance().BindModel(x, y, false);
	entt::entity z = CreateCoordinateAxesZ();
	SceneGraph::GetInstance().BindModel(x, z, false);
	return true;
}

entt::entity CEngine::CreateCoordinateAxesX()
{
	// 坐标轴圆柱体
	CModel::S_MODEL_DESC descColumn;
	descColumn.strName = "COLUMN_X";
	descColumn.S_MODEL_COLUMN_DESC.start = glm::vec3(0.0f);
	descColumn.S_MODEL_COLUMN_DESC.normal = glm::vec3(1.0f, 0.0f, 0.0f);
	descColumn.S_MODEL_COLUMN_DESC.length = 5.0f;
	descColumn.S_MODEL_COLUMN_DESC.radius = 0.05f;
	descColumn.S_MODEL_COLUMN_DESC.sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	descColumn.S_MODEL_COLUMN_DESC.eColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	std::shared_ptr<CModel> AxesXColumn = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, descColumn);
	if (nullptr == AxesXColumn) {
		PRINTLOG("Fail to create MoAxesXdel");
		return entt::null;
	}
	entt::entity entityColumn = SceneGraph::GetInstance().CreateModel(*AxesXColumn.get());

	CModel::S_MODEL_DESC descArrow;
	descArrow.strName = "CONE_X";
	descArrow.S_MODEL_CONE_DESC.center = descColumn.S_MODEL_COLUMN_DESC.start + 
		glm::normalize(descColumn.S_MODEL_COLUMN_DESC.normal) * descColumn.S_MODEL_COLUMN_DESC.length;
	descArrow.S_MODEL_CONE_DESC.axisDir = descColumn.S_MODEL_COLUMN_DESC.normal;
	descArrow.S_MODEL_CONE_DESC.radius = descColumn.S_MODEL_COLUMN_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.height = descArrow.S_MODEL_CONE_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.color = descColumn.S_MODEL_COLUMN_DESC.sColor;
	std::shared_ptr<CModel> AxesXArrow = CModelLoader::LoadModel(CModel::E_MODEL_CONE, descArrow);
	if (nullptr == AxesXArrow) {
		PRINTLOG("Fail to load Model");
		return entt::null;
	}
	entt::entity entityArrow = SceneGraph::GetInstance().CreateModel(*AxesXArrow.get());
	SceneGraph::GetInstance().BindModel(entityColumn, entityArrow);
	return entityColumn;
}

entt::entity CEngine::CreateCoordinateAxesY()
{
	// 坐标轴圆柱体
	CModel::S_MODEL_DESC descColumn;
	descColumn.strName = "COLUMN_Y";
	descColumn.S_MODEL_COLUMN_DESC.start = glm::vec3(0.0f);
	descColumn.S_MODEL_COLUMN_DESC.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	descColumn.S_MODEL_COLUMN_DESC.length = 5.0f;
	descColumn.S_MODEL_COLUMN_DESC.radius = 0.05f;
	descColumn.S_MODEL_COLUMN_DESC.sColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	descColumn.S_MODEL_COLUMN_DESC.eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	std::shared_ptr<CModel> AxesXColumn = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, descColumn);
	if (nullptr == AxesXColumn) {
		PRINTLOG("Fail to create MoAxesXdel");
		return entt::null;
	}
	entt::entity entityColumn = SceneGraph::GetInstance().CreateModel(*AxesXColumn.get());

	CModel::S_MODEL_DESC descArrow;
	descArrow.strName = "CONE_Y";
	descArrow.S_MODEL_CONE_DESC.center = descColumn.S_MODEL_COLUMN_DESC.start + 
		glm::normalize(descColumn.S_MODEL_COLUMN_DESC.normal) * descColumn.S_MODEL_COLUMN_DESC.length;
	descArrow.S_MODEL_CONE_DESC.axisDir = descColumn.S_MODEL_COLUMN_DESC.normal;
	descArrow.S_MODEL_CONE_DESC.radius = descColumn.S_MODEL_COLUMN_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.height = descArrow.S_MODEL_CONE_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.color = descColumn.S_MODEL_COLUMN_DESC.sColor;
	std::shared_ptr<CModel> AxesXArrow = CModelLoader::LoadModel(CModel::E_MODEL_CONE, descArrow);
	if (nullptr == AxesXArrow) {
		PRINTLOG("Fail to load Model");
		return entt::null;
	}
	entt::entity entityArrow = SceneGraph::GetInstance().CreateModel(*AxesXArrow.get());
	SceneGraph::GetInstance().BindModel(entityColumn, entityArrow);
	return entityColumn;
}

entt::entity CEngine::CreateCoordinateAxesZ()
{
	// 坐标轴圆柱体
	CModel::S_MODEL_DESC descColumn;
	descColumn.strName = "COLUMN_Z";
	descColumn.S_MODEL_COLUMN_DESC.start = glm::vec3(0.0f);
	descColumn.S_MODEL_COLUMN_DESC.normal = glm::vec3(0.0f, 0.0f, 1.0f);
	descColumn.S_MODEL_COLUMN_DESC.length = 5.0f;
	descColumn.S_MODEL_COLUMN_DESC.radius = 0.05f;
	descColumn.S_MODEL_COLUMN_DESC.sColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	descColumn.S_MODEL_COLUMN_DESC.eColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	std::shared_ptr<CModel> AxesXColumn = CModelLoader::LoadModel(CModel::E_MODEL_COLUMN, descColumn);
	if (nullptr == AxesXColumn) {
		PRINTLOG("Fail to create MoAxesXdel");
		return entt::null;
	}
	entt::entity entityColumn = SceneGraph::GetInstance().CreateModel(*AxesXColumn.get());

	CModel::S_MODEL_DESC descArrow;
	descArrow.strName = "CONE_Z";
	descArrow.S_MODEL_CONE_DESC.center = descColumn.S_MODEL_COLUMN_DESC.start +
		glm::normalize(descColumn.S_MODEL_COLUMN_DESC.normal) * descColumn.S_MODEL_COLUMN_DESC.length;
	descArrow.S_MODEL_CONE_DESC.axisDir = descColumn.S_MODEL_COLUMN_DESC.normal;
	descArrow.S_MODEL_CONE_DESC.radius = descColumn.S_MODEL_COLUMN_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.height = descArrow.S_MODEL_CONE_DESC.radius * 2;
	descArrow.S_MODEL_CONE_DESC.color = descColumn.S_MODEL_COLUMN_DESC.sColor;
	std::shared_ptr<CModel> AxesXArrow = CModelLoader::LoadModel(CModel::E_MODEL_CONE, descArrow);
	if (nullptr == AxesXArrow) {
		PRINTLOG("Fail to load Model");
		return entt::null;
	}
	entt::entity entityArrow = SceneGraph::GetInstance().CreateModel(*AxesXArrow.get());
	SceneGraph::GetInstance().BindModel(entityColumn, entityArrow);
	return entityColumn;
}
