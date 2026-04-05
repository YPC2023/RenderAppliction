#include "CMaterialSystem.h"

std::shared_ptr<CMaterial> CMaterialSystem::AquireMaterial(const CMaterial::S_MATERIAL_DESC& desc)
{
	return std::make_shared<CMaterial>(desc);
}