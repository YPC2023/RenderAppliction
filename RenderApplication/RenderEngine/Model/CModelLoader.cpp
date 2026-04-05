#include "CModelLoader.h"
#include <CUtils.h>

std::shared_ptr<CModel> CModelLoader::LoadModel(CModel::E_MODEL_TYPE type, const CModel::S_MODEL_DESC& desc)
{
	std::shared_ptr<CModel> model = std::make_shared<CModel>(type, desc);
	if (nullptr == model) {
		PRINTLOG("Fail to load model");
		return nullptr;
	}
	return model;
}