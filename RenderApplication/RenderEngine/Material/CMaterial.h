#pragma once
#include <vector>
#include <memory>
#include "CShader.h"
#include "../Resource/CResourceManager.h"

class CMaterial
{
public:
	typedef struct _S_MATERIAL_TEXTURE
	{
		std::string					strType;
		std::shared_ptr<CTexture>	texture;
	}S_MATERIAL_TEXTURE;
	typedef struct _S_MATERIAL_DESC : public CShader::S_SHADER_DESC
	{
		std::vector<S_MATERIAL_TEXTURE>	textures;
	}S_MATERIAL_DESC;
public:
	//using S_MATERIAL_DESC = CShader::S_SHADER_DESC;
public:
	CMaterial(const S_MATERIAL_DESC& desc);

public:
	const S_MATERIAL_DESC& GetDesc() const { return m_desc; }
	bool IsOK() const { return m_ok; }
private:
	bool SetupMaterial();
public:
	S_MATERIAL_DESC	m_desc;
	bool			m_ok;
public:
	std::shared_ptr<CShader>		m_shader;
};