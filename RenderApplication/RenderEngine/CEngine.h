#pragma once
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
private:
	void AppendModel(const CModel& model);
private:
	bool CreateModelChess();
	bool CreateModelColumn();
	bool CreateModelSphere();
	bool CreateModelCone();
	bool CreateModelTorus();
};