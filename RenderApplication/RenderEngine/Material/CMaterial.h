#pragma once
#include <vector>
#include <memory>
#include "CShader.h"
#include "../Resource/CTexture.h"

class CMaterial
{
public:
	typedef struct _S_SHADER_TEXTURE
	{
		std::string					strType;
		std::shared_ptr<CTexture>	texture;
		bool operator<(const _S_SHADER_TEXTURE& desc) const {
			return (std::tie(strType, texture->GetDesc().strPath)
				< std::tie(desc.strType, desc.texture->GetDesc().strPath));
		}
	}S_SHADER_TEXTURE;
	typedef struct _S_MATERIAL_DESC : CShader::S_SHADER_DESC
	{
		std::vector<S_SHADER_TEXTURE> textures;
		bool operator<(const _S_MATERIAL_DESC& desc) const {
			return std::tie(hasMVP, hasPosition, hasColor, hasNormal, hasTexCoords, hasTangent, hasBitangent,
				hasSelected, renderID, textures) <
				std::tie(desc.hasMVP, desc.hasPosition, desc.hasColor, desc.hasNormal, desc.hasTexCoords,
					desc.hasTangent, desc.hasBitangent, desc.hasSelected, desc.renderID, desc.textures);
		}
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