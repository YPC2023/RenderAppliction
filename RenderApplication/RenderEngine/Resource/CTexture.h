#pragma once
#include <glad/glad.h>
#include <string>
#include <tuple>

class CTexture
{
public:
	typedef struct _S_TEXTURE_DESC
	{
		int width = 0;
		int height = 0;
		std::string strPath = "";
		unsigned int Format = GL_RGB;
		unsigned int TEX_GL_TEXTURE_WRAP_S = GL_REPEAT;
		unsigned int TEX_GL_TEXTURE_WRAP_T = GL_REPEAT;
		unsigned int TEX_GL_TEXTURE_MIN_FILTER = GL_LINEAR;
		unsigned int TEX_GL_TEXTURE_MAG_FILTER = GL_LINEAR;
		bool operator<(const _S_TEXTURE_DESC& desc) const {
			return std::tie(width, height, strPath, Format, 
				TEX_GL_TEXTURE_WRAP_S, TEX_GL_TEXTURE_WRAP_T, 
				TEX_GL_TEXTURE_MIN_FILTER, TEX_GL_TEXTURE_MAG_FILTER) <
				std::tie(desc.width, desc.height, desc.strPath, desc.Format,
					desc.TEX_GL_TEXTURE_WRAP_S, desc.TEX_GL_TEXTURE_WRAP_T,
					desc.TEX_GL_TEXTURE_MIN_FILTER, desc.TEX_GL_TEXTURE_MAG_FILTER);
		}
	}S_TEXTURE_DESC;

public:
	CTexture(const S_TEXTURE_DESC& desc);
	~CTexture();
private:
	bool SetupTexture();
	bool LoadFromFile();
	bool CreateEmptyTexture();
public:
	void Resize(int width, int height);
	bool SaveToFile(const std::string& strPath);
public:
	bool IsOK() { return m_ok; }
	const S_TEXTURE_DESC& GetDesc() { return m_desc; }
	unsigned int GetID() { return m_ID; }
private:
	S_TEXTURE_DESC	m_desc;
	bool			m_ok;
private:
	unsigned int	m_ID;
};