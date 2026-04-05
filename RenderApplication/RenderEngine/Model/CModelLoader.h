#pragma once
#include <memory>
#include "CModel.h"
#include "../Material/CMaterialSystem.h"

class CModelLoader
{
public:
	static std::shared_ptr<CModel> LoadModel(CModel::E_MODEL_TYPE type, const CModel::S_MODEL_DESC& desc);
	static CMaterial::S_MATERIAL_DESC TranslateMaterial(const CMesh& mesh);
};