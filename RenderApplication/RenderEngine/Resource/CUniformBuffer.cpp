#include "CUniformBuffer.h"
#include <glad/glad.h>

CUniformBuffer::CUniformBuffer(const S_UNIFORMBUFFER_DESC& desc)
{
	m_desc = desc;
	m_ok = SetupUniformBuffer();
}

void CUniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
}

void CUniformBuffer::UnBind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CUniformBuffer::SendData(const void* data, unsigned int size, unsigned int offset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
    // 使用 SubData 进行局部内存拷贝，效率最高
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

bool CUniformBuffer::SetupUniformBuffer()
{
    // 1. 生成 Buffer
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ID);

    // 2. 分配空间 (GL_DYNAMIC_DRAW 适合每帧更新的数据)
    glBufferData(GL_UNIFORM_BUFFER, m_desc.size, NULL, GL_DYNAMIC_DRAW);

    // 3. 将 Buffer 绑定到特定的 Binding Point (插座)
    glBindBufferBase(GL_UNIFORM_BUFFER, m_desc.BindingPoint, m_ID);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return true;
}

