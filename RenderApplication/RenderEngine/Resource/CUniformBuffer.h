#pragma once
#include <string>
#include "../Core/Define.h"

class CUniformBuffer
{
public:
	typedef struct _S_UNIFORMBUFFER_DESC
	{
		unsigned int			size = 0;
		UBO_BINDINGPOINT_INDEX	BindingPoint = UBO_BINDINGPOINT_MVP;
	}S_UNIFORMBUFFER_DESC;

public:
	CUniformBuffer(const S_UNIFORMBUFFER_DESC& desc);
	// 禁用拷贝，确保 GPU 资源唯一性
	CUniformBuffer(const CUniformBuffer&) = delete;
	CUniformBuffer& operator=(const CUniformBuffer&) = delete;

public:
	void Bind();
	void UnBind();
	void SendData(const void* data, unsigned int size, unsigned int offset = 0);
private:
	bool SetupUniformBuffer();

public:
	S_UNIFORMBUFFER_DESC	m_desc;
	bool					m_ok;
public:
	HUBO					m_ID;
};