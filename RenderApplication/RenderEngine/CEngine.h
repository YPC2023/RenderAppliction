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
	bool MergeModel(const std::set<entt::entity>& setModel);
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