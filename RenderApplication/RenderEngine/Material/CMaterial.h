#pragma once
#include <memory>
#include "CShader.h"

class CMaterial
{
public:
	using S_MATERIAL_DESC = CShader::S_SHADER_DESC;
public:
	CMaterial(const S_MATERIAL_DESC& desc);

public:
	const S_MATERIAL_DESC& GetDesc() const { return m_desc; }
	bool IsOK() const { return m_ok; }
private:
	bool SetupMaterial();
private:
	S_MATERIAL_DESC	m_desc;
	bool			m_ok;
public:
	std::shared_ptr<CShader>	m_shader;
};