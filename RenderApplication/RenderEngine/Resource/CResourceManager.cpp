#include "CResourceManager.h"

std::map<CTexture::S_TEXTURE_DESC, std::set<std::shared_ptr<CTexture>>>				CResourceManager::m_map_Texture;
std::map<CFramebuffer::S_FRAMEBUFFER_DESC, std::set<std::shared_ptr<CFramebuffer>>>	CResourceManager::m_map_Framebuffer;
std::map<unsigned int, std::shared_ptr<CUniformBuffer>> CResourceManager::m_map_UniformBuffer;

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

void CResourceManager::DestroyFrameBuffer(std::shared_ptr<CFramebuffer> buffer)
{
	if (0 < m_map_Framebuffer.count(buffer->GetDesc()) &&
		0 < m_map_Framebuffer[buffer->GetDesc()].count(buffer)) {
		m_map_Framebuffer[buffer->GetDesc()].erase(buffer);
	}
}

std::shared_ptr<CUniformBuffer> CResourceManager::AquireUnifrombuffer(const CUniformBuffer::S_UNIFORMBUFFER_DESC& desc)
{
	if (0 < m_map_UniformBuffer.count(desc.BindingPoint)) {
		return m_map_UniformBuffer[desc.BindingPoint];
	}
	std::shared_ptr<CUniformBuffer> buffer = std::make_shared<CUniformBuffer>(desc);

	m_map_UniformBuffer[desc.BindingPoint] = buffer;
	return buffer;
}

std::shared_ptr<CUniformBuffer> CResourceManager::AquireUnifrombuffer(UBO_BINDINGPOINT_INDEX binding)
{
	if (0 < m_map_UniformBuffer.count(binding)) {
		return m_map_UniformBuffer[binding];
	}
	return nullptr;
}

void CResourceManager::DestroyUniformBuffer(std::shared_ptr<CUniformBuffer> buffer)
{
	if (0 < m_map_UniformBuffer.count(buffer->m_desc.BindingPoint)) {
		m_map_UniformBuffer.erase(buffer->m_desc.BindingPoint);
	}
}