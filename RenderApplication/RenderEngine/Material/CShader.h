#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <inja/inja.hpp>
#include "../Resource/CResourceManager.h"

class CMaterial;

class CShader
{
public:
	typedef struct _S_SHADER_MVP_MATRIX
	{
		glm::mat4 projection;
		glm::mat4 view;
	}S_SHADER_MVP_MATRIX;

	typedef struct _S_SHADER_DESC
	{
		bool hasMVP = true;
		bool hasMVP_UBO = true;	// 通过UBO设置投影矩阵和视图矩阵
		bool hasPosition = true;
		bool hasColor = false;
		bool hasNormal = false;
		bool hasTexCoords = false;
		bool hasTangent = false;
		bool hasBitangent = false;
		bool hasSelected = true;
		bool renderID = false;
	}S_SHADER_DESC;
public:
	CShader(const S_SHADER_DESC& desc);
	CShader(const std::string& vertex, const std::string& fragment);
	~CShader();
private:
	bool SetupShader();
private:
	inja::json ShaderDescToInjaJson();
	std::string RenderShaderCode(const std::string& strPath, const inja::json& data);
	bool CreateShader(const std::string& textex, const std::string& fragment);
	bool CheckCompileResult(unsigned int id);
	bool CheckLinkResult(unsigned int id);
	bool CompileVertex(const std::string& strCode);
	bool CompileFragment(const std::string& strCode);
	bool LinkProgram();
private:
	bool UBO_MVP_Create();
private: // UBO
	bool UBO_BindingPoint(const std::string& strNeme, unsigned int binding);
public:
	bool IsOK() { return m_ok; } 
	const S_SHADER_DESC& GetDesc() { return m_desc; }
public: // UBO
	bool UBO_MVP_BindingPoint(unsigned int binding);
public:
	void use() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
	S_SHADER_DESC	m_desc;
	bool			m_ok;
private:
	std::string		m_strVertexPath;
	std::string		m_strFragmentPath;
	std::string		m_strVertexCode;
	std::string		m_strFragmentCode;
	unsigned int	m_VertexId;
	unsigned int	m_FragmentId;
	unsigned int	m_ID;
public:
	std::shared_ptr<CUniformBuffer> m_UBO_MVPMatrix;
};