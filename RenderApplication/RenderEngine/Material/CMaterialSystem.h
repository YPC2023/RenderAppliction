#pragma once
#include <memory>
#include <map>
#include "CMaterial.h"

class CMaterialSystem
{
public:
	static std::shared_ptr<CMaterial> AquireMaterial(const CMaterial::S_MATERIAL_DESC& desc);

private:
	static std::map<CMaterial::S_MATERIAL_DESC, std::shared_ptr<CMaterial>>	m_map_Material;
};