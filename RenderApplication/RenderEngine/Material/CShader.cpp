#include "CShader.h"
#include <glad/glad.h>
#include <CUtils.h>
#include "../Core/ConfigSystem.h"

CShader::CShader(const S_SHADER_DESC& desc)
{
	m_desc = desc;
	m_VertexId = 0;
	m_FragmentId = 0;
	m_ID = 0;
	m_ok = SetupShader();
}

CShader::CShader(const std::string& vertex, const std::string& fragment)
{
	m_ok = CreateShader(vertex, fragment);
}

CShader::~CShader()
{
	if (0 != m_VertexId) {
		glDeleteShader(m_VertexId);
	}
	if (0 != m_FragmentId) {
		glDeleteShader(m_FragmentId);
	}
	if (0 != m_ID) {
		glDeleteProgram(m_ID);
	}
}

bool CShader::SetupShader()
{
	m_strVertexPath = ConfigSystem::GetDefaultVertexTemplateFilePath();
	m_strFragmentPath = ConfigSystem::GetDefaultFragmentTemplateFilePath();
	if (!CUtils::FileExists(m_strVertexPath.c_str())) {
		PRINTLOG("Path of vertex is not exists(%s)", m_strVertexPath.c_str());
		return false;
	}
	if (!CUtils::FileExists(m_strFragmentPath.c_str())) {
		PRINTLOG("Path of fragment is not exists(%s)", m_strFragmentPath.c_str());
		return false;
	}
	m_strVertexCode = RenderShaderCode(m_strVertexPath, ShaderDescToInjaJson());
	if ("" == m_strVertexCode) {
		PRINTLOG("Fail to render vertex code(template=%s)", m_strVertexPath.c_str());
		return false;
	}
	m_strFragmentCode = RenderShaderCode(m_strFragmentPath, ShaderDescToInjaJson());
	if ("" == m_strFragmentCode) {
		PRINTLOG("Fail to render fragment code(template=%s)", m_strFragmentPath.c_str());
		return false;
	}
	
	if (!CreateShader(m_strVertexCode, m_strFragmentCode)) {
		PRINTLOG("Fail to create shader");
		return false;
	}

	// 创建MVPMatrix
	if (m_desc.hasMVP_UBO) {
		if (!UBO_MVP_Create()) {
			PRINTLOG("Fail to create UBO of MVP");
			return false;
		}
	}
	return true;
}

inja::json CShader::ShaderDescToInjaJson()
{
	inja::json json;

	json["hasMVP"] = m_desc.hasMVP;
	json["hasMVP_UBO"] = m_desc.hasMVP_UBO;
	json["hasPosition"] = m_desc.hasPosition;
	json["hasColor"] = m_desc.hasColor;
	json["hasNormal"] = m_desc.hasNormal;
	json["hasTexCoords"] = m_desc.hasTexCoords;
	json["hasTangent"] = m_desc.hasTangent;
	json["hasBitangent"] = m_desc.hasBitangent;
	json["hasSelected"] = m_desc.hasSelected;
	json["renderID"] = m_desc.renderID;

	return json;
}

std::string CShader::RenderShaderCode(const std::string& strPath, const inja::json& data)
{
	if (!CUtils::FileExists(strPath.c_str())) {
		PRINTLOG("Path is not exists(%s)", strPath.c_str());
		return "";
	}
	inja::Environment env;
	try {
		return env.render_file(strPath, data);
	}
	catch (const inja::InjaError& e) {
		// 捕获所有 Inja 相关的错误（基类）
		std::cerr << "渲染失败: " << e.what() << std::endl;

		// 如果需要更精细的信息，可以访问公共成员：
		// e.type: 错误类型 (如 "render_error", "parser_error")
		// e.location: 错误发生的行号和列号
		std::cerr << "位置: " << e.location.line << ":" << e.location.column << std::endl;
		PRINTLOG("Fail render template(%s)\n[%d,%d] %s", strPath.c_str(), e.what(), e.location.line, e.location.column);
	}
	return "";
}

bool CShader::CreateShader(const std::string& textex, const std::string& fragment)
{
	if (!CompileVertex(textex)) {
		PRINTLOG("Fail to compile vertex");
		return false;
	}
	if (!CompileFragment(fragment)) {
		PRINTLOG("Fail to compile fragment");
		return false;
	}
	if (!LinkProgram()) {
		PRINTLOG("Fail to link program");
		return false;
	}
	return true;
}

bool CShader::CheckCompileResult(unsigned int id)
{
	GLint success;
	GLchar infoLog[1024] = { 0 };
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success) {
		return true;
	}
	glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
	PRINTLOG("ERROR: Fail to compile [%s]:\n%s", 
		m_VertexId == id ? 
		("" != m_strVertexPath ? m_strVertexPath.c_str() : m_strVertexCode.c_str()) :
		("" != m_strFragmentPath ? m_strFragmentPath.c_str() : m_strFragmentCode.c_str()),
		infoLog);
	return true;
}

bool CShader::CheckLinkResult(unsigned int id)
{
	GLint success;
	GLchar infoLog[1024] = { 0 };
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (success) {
		return true;
	}
	glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
	PRINTLOG("ERROR: Fail to link Shader\n%s", infoLog);
	return true;
}

bool CShader::CompileVertex(const std::string& strCode)
{
	const char* buf = strCode.c_str();
	// vertex shader
	m_VertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexId, 1, &buf, NULL);
	glCompileShader(m_VertexId);
	return CheckCompileResult(m_VertexId);
}

bool CShader::CompileFragment(const std::string& strCode)
{
	const char* buf = strCode.c_str();
	// vertex shader
	m_FragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentId, 1, &buf, NULL);
	glCompileShader(m_FragmentId);
	return CheckCompileResult(m_FragmentId);
}

bool CShader::LinkProgram()
{
	m_ID = glCreateProgram();
	glAttachShader(m_ID, m_VertexId);
	glAttachShader(m_ID, m_FragmentId);
	glLinkProgram(m_ID);
	return CheckLinkResult(m_ID);
}

bool CShader::UBO_MVP_Create()
{
	CUniformBuffer::S_UNIFORMBUFFER_DESC desc;
	desc.BindingPoint = UBO_BINDINGPOINT_MVP;
	desc.size = sizeof(S_SHADER_MVP_MATRIX);
	m_UBO_MVPMatrix = CResourceManager::AquireUnifrombuffer(desc);
	if (nullptr == m_UBO_MVPMatrix) {
		PRINTLOG("Fail to create UBO of MVP");
		return false;
	}
	if (!UBO_MVP_BindingPoint(desc.BindingPoint)) {
		PRINTLOG("Fail to bind MVP BindingPoint");
		return false;
	}
	return true;
}

bool CShader::UBO_BindingPoint(const std::string& strNeme, unsigned int binding)
{
	unsigned int blockIndex = glGetUniformBlockIndex(m_ID, strNeme.c_str());
	if (GL_INVALID_INDEX == blockIndex) {
		return false;
	}
	glUniformBlockBinding(m_ID, blockIndex, binding);
	return true;
}

bool CShader::UBO_MVP_BindingPoint(unsigned int binding)
{
	return UBO_BindingPoint("MVPMatrix", binding);
}

void CShader::use() const
{
	glUseProgram(m_ID);
}

void CShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void CShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CShader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void CShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void CShader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void CShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void CShader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void CShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void CShader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CShader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}