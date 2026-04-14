#pragma once
#include <entt/entt.hpp>
#include <ISingletonInterface.h>
#include "IEngineInterface.h"
#include "Model/CModel.h"
#include "Material/CMaterialSystem.h"


class CEngine : public IEngineInterface
{
private:
	static CEngine* m_pInstance;
	// 1. 构造函数私有化，禁止外部通过 new 或 栈声明创建对象
	CEngine();
	~CEngine();
public:
	// 3. 禁用拷贝构造和赋值操作，防止通过副本创建新实例
	CEngine(const CEngine&) = delete;
	CEngine& operator=(const CEngine&) = delete;
	static CEngine& GetInstance();
	static void ReleaseIntance();
public:
	bool Initialize() override;
	void UnInitialize() override;
	bool SetLoader(GLADloadproc loader) override;
	bool LoadModel(const char* path) override;
public:
	bool MergeModel(const std::set<entt::entity>& setModel);
private:
	bool CreateModelChess();
	bool CreateModelColumn();
	bool CreateModelSphere();
	bool CreateModelCone();
	bool CreateModelTorus();
	bool CreateModelRingArc();
	bool CreateCoordinateAxes();
	entt::entity CreateCoordinateAxesX();
	entt::entity CreateCoordinateAxesY();
	entt::entity CreateCoordinateAxesZ();
};