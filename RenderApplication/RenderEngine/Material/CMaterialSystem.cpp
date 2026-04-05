#include "CMaterialSystem.h"


std::map<CMaterial::S_MATERIAL_DESC, std::shared_ptr<CMaterial>>	CMaterialSystem::m_map_Material;

std::shared_ptr<CMaterial> CMaterialSystem::AquireMaterial(const CMaterial::S_MATERIAL_DESC& desc)
{
	if (0 < m_map_Material.count(desc)) {
		return m_map_Material[desc];
	}
	m_map_Material[desc] = std::make_shared<CMaterial>(desc);
	return m_map_Material[desc];
}