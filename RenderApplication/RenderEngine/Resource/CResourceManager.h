#pragma once
#include <map>
#include <set>
#include <memory>
#include "CTexture.h"
#include "CFramebuffer.h"

class CResourceManager
{

public:
	static std::shared_ptr<CTexture> AquireTexture(const CTexture::S_TEXTURE_DESC& desc);
	static void DestroyTexture(std::shared_ptr<CTexture> texture);

	static std::shared_ptr<CFramebuffer> AquireFramebuffer(const CFramebuffer::S_FRAMEBUFFER_DESC& desc);
	static void DestroyTexture(std::shared_ptr<CFramebuffer> framebuffer);
private:
	static std::map<CTexture::S_TEXTURE_DESC, std::set<std::shared_ptr<CTexture>>>				m_map_Texture;
	static std::map<CFramebuffer::S_FRAMEBUFFER_DESC, std::set<std::shared_ptr<CFramebuffer>>>	m_map_Framebuffer;
};