#include "CModelLoader.h"
#include <CUtils.h>

std::shared_ptr<CModel> CModelLoader::LoadFileModel(const char* path, bool bAutoRecycle)
{
	std::shared_ptr<CModel> model = std::make_shared<CModel>(path, bAutoRecycle);
	if (nullptr == model) {
		PRINTLOG("Fail to load model file(%s)", path);
		return nullptr;
	}
	return model;
}