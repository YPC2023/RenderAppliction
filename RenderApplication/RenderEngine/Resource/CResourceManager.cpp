#include "CResourceManager.h"

std::map<CTexture::S_TEXTURE_DESC, std::set<std::shared_ptr<CTexture>>>				CResourceManager::m_map_Texture;
std::map<CFramebuffer::S_FRAMEBUFFER_DESC, std::set<std::shared_ptr<CFramebuffer>>>	CResourceManager::m_map_Framebuffer;

std::shared_ptr<CTexture> CResourceManager::AquireTexture(const CTexture::S_TEXTURE_DESC& desc)
{
	if (0 < m_map_Texture.count(desc) && 0 < m_map_Texture[desc].size()) {
		// 1. 获取第一个元素
		std::shared_ptr<CTexture> texture = *m_map_Texture[desc].begin();

		// 2. 从集合中移除该元素
		m_map_Texture[desc].erase(m_map_Texture[desc].begin());
		return texture;
	}
	return std::make_shared<CTexture>(desc);
}

void CResourceManager::DestroyTexture(std::shared_ptr<CTexture> texture)
{
	if (0 < m_map_Texture.count(texture->GetDesc()) && 
		0 < m_map_Texture[texture->GetDesc()].count(texture)) {
		m_map_Texture[texture->GetDesc()].erase(texture);
	}
}


std::shared_ptr<CFramebuffer> CResourceManager::AquireFramebuffer(const CFramebuffer::S_FRAMEBUFFER_DESC& desc)
{
	if (0 < m_map_Framebuffer.count(desc) && 0 < m_map_Framebuffer[desc].size()) {
		// 1. 获取第一个元素
		std::shared_ptr<CFramebuffer> framebuffer = *m_map_Framebuffer[desc].begin();

		// 2. 从集合中移除该元素
		m_map_Framebuffer[desc].erase(m_map_Framebuffer[desc].begin());
		return framebuffer;
	}
	return std::make_shared<CFramebuffer>(desc);
}

void CResourceManager::DestroyTexture(std::shared_ptr<CFramebuffer> framebuffer)
{
	if (0 < m_map_Framebuffer.count(framebuffer->GetDesc()) &&
		0 < m_map_Framebuffer[framebuffer->GetDesc()].count(framebuffer)) {
		m_map_Framebuffer[framebuffer->GetDesc()].erase(framebuffer);
	}
}
