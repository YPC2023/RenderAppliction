#pragma once
#include <memory>
#include "CModel.h"

class CModelLoader
{
public:
	static std::shared_ptr<CModel> LoadModel(const char* path, bool bAutoRecycle = true);
};