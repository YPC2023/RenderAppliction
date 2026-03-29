#pragma once
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include "CTexture.h"

class CFramebuffer
{
public:
	typedef struct _S_FRAMEBUFFER_DESC
	{
		int width;
		int height;
		std::vector<CTexture::S_TEXTURE_DESC>	vecTexture;
		unsigned int DepthType = GL_DEPTH24_STENCIL8;	// 深度缓冲类型
		// 重载 < 运算符
		bool operator<(const _S_FRAMEBUFFER_DESC& desc) const {
			return std::tie(width, height, vecTexture, DepthType) <
				std::tie(desc.width, desc.height, desc.vecTexture, desc.DepthType);
		}

	}S_FRAMEBUFFER_DESC;
public:
	CFramebuffer(const S_FRAMEBUFFER_DESC& desc);
	~CFramebuffer();
private:
	bool SetupFramebuffer();
public:
	const S_FRAMEBUFFER_DESC& GetDesc() { return m_desc; }
	bool IsOK() { return m_ok; }
	unsigned int GetFBO() { return m_FBO; }
	unsigned int GetDBO() { return m_DBO; }
	std::shared_ptr<CTexture> GetTexture(size_t index);
	size_t GetTextureSize() { return m_CBO.size(); }
	void Resize(int width, int height);
public:
	void Bind();
	void UnBind();
private:
	S_FRAMEBUFFER_DESC	m_desc;
	bool				m_ok;
private:
	unsigned int							m_FBO;
	unsigned int							m_DBO;
	std::vector<std::shared_ptr<CTexture>>	m_CBO;

};