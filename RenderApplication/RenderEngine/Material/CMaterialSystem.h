#pragma once
#include <memory>
#include "CMaterial.h"

class CMaterialSystem
{
public:
	static std::shared_ptr<CMaterial> AquireMaterial(const CMaterial::S_MATERIAL_DESC& desc);
};