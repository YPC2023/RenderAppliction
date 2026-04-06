#pragma once
#include <entt/entt.hpp>
#include <ISingletonInterface.h>
#include "IEngineInterface.h"
#include "Model/CModel.h"
#include "Material/CMaterialSystem.h"


class CEngine : public IEngineInterface,
	public ISingletonInterface<CEngine>
{
public:
	bool Initialize() override;
	void UnInitialize() override;
	bool SetLoader(GLADloadproc loader) override;
	bool LoadModel(const char* path) override;
public:
	bool MergeModel(entt::entity parent, entt::entity child);
private:
	entt::entity AppendModel(const CModel& model);
	// 把child绑定到parent上
	void BindModel(entt::entity parent, entt::entity child);
	// 把entity从当前它所绑定的节点上解绑
	void UnBindModel(entt::entity entity);
private:
	bool CreateModelChess();
	bool CreateModelColumn();
	bool CreateModelSphere();
	bool CreateModelCone();
	bool CreateModelTorus();
	bool CreateCoordinateAxes();
	entt::entity CreateCoordinateAxesX();
	entt::entity CreateCoordinateAxesY();
	entt::entity CreateCoordinateAxesZ();
};