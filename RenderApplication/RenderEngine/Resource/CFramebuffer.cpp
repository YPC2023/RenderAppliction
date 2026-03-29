#include "CFramebuffer.h"
#include "CResourceManager.h"
#include <CUtils.h>

CFramebuffer::CFramebuffer(const S_FRAMEBUFFER_DESC& desc)
{
	m_FBO = 0;
	m_DBO = 0;
	m_desc = desc;
	m_ok = SetupFramebuffer();
}

CFramebuffer::~CFramebuffer()
{
	m_CBO.clear();

	if (0 != m_DBO) {
		glDeleteRenderbuffers(1, &m_DBO);
	}
	if (0 != m_FBO) {
		glDeleteRenderbuffers(1, &m_FBO);
	}
}

bool CFramebuffer::SetupFramebuffer()
{
	// 1. 创建帧缓冲
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// 2. 创建纹理附件
	for (size_t index = 0; index < m_desc.vecTexture.size(); ++index) {
		m_CBO.push_back(CResourceManager::AquireTexture(m_desc.vecTexture[index]));
		// 将纹理附加到帧缓冲的颜色附件
		glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + index), GL_TEXTURE_2D, m_CBO[m_CBO.size()-1]->GetID(), 0);
	}
	// 3. 创建渲染缓冲对象（用于深度和模板附件）
	if (0 != m_desc.DepthType) {
		glGenRenderbuffers(1, &m_DBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DBO);
		glRenderbufferStorage(GL_RENDERBUFFER, m_desc.DepthType, m_desc.width, m_desc.height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// 将渲染缓冲附加到帧缓冲
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DBO);
	}
	
	// 4. 检查帧缓冲完整性
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		PRINTLOG("Fail to crate framebuffer");
		return false;
	}

	// 5. 解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

std::shared_ptr<CTexture> CFramebuffer::GetTexture(size_t index)
{
	if (index < m_CBO.size()) {
		return m_CBO[index];
	}
	return nullptr;
}

void CFramebuffer::Resize(int width, int height)
{
	for (size_t index = 0; index < m_CBO.size(); ++index) {
		m_CBO[index]->Resize(width, height);
	}
	// 更新深度模板缓冲
	glBindRenderbuffer(GL_RENDERBUFFER, m_DBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

void CFramebuffer::Bind()
{
	if (0 != m_FBO) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}
}

void CFramebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
