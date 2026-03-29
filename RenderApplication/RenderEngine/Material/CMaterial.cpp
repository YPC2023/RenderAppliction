#include "CMaterial.h"
#include <CUtils.h>

CMaterial::CMaterial(const S_MATERIAL_DESC& desc)
{
	m_desc = desc;
	m_ok = SetupMaterial();
}

bool CMaterial::SetupMaterial()
{
	m_shader = std::make_shared<CShader>(m_desc);
	if (nullptr == m_shader && !m_shader->IsOK()) {
		PRINTLOG("Fail to new shader");
		return false;
	}
	return true;
}