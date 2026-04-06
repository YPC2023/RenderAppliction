#pragma once
#include <map>
#include <set>
#include <memory>
#include "CTexture.h"
#include "CFramebuffer.h"
#include "CUniformBuffer.h"

class CResourceManager
{
public:
	static std::shared_ptr<CTexture> AquireTexture(const CTexture::S_TEXTURE_DESC& desc);
	static void DestroyTexture(std::shared_ptr<CTexture> texture);

	static std::shared_ptr<CFramebuffer> AquireFramebuffer(const CFramebuffer::S_FRAMEBUFFER_DESC& desc);
	static void DestroyFrameBuffer(std::shared_ptr<CFramebuffer> buffer);

	static std::shared_ptr<CUniformBuffer> AquireUnifrombuffer(const CUniformBuffer::S_UNIFORMBUFFER_DESC& desc);
	static std::shared_ptr<CUniformBuffer> AquireUnifrombuffer(UBO_BINDINGPOINT_INDEX binding);
	static void DestroyUniformBuffer(std::shared_ptr<CUniformBuffer> buffer);
private:
	static std::map<CTexture::S_TEXTURE_DESC, std::set<std::shared_ptr<CTexture>>>				m_map_Texture;
	static std::map<CFramebuffer::S_FRAMEBUFFER_DESC, std::set<std::shared_ptr<CFramebuffer>>>	m_map_Framebuffer;
	static std::map<unsigned int, std::shared_ptr<CUniformBuffer>> m_map_UniformBuffer;
};