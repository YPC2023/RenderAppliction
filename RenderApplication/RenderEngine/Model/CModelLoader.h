#pragma once
#include <memory>
#include "CModel.h"

class CModelLoader
{
public:
	static std::shared_ptr<CModel> LoadModel(CModel::E_MODEL_TYPE type, const CModel::S_MODEL_DESC& desc);
};