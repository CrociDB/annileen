#include "mesh.h"

using namespace annileen;

void Mesh::init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout, const bgfx::Memory* indexData, uint16_t indexCount)
{
	m_VertexData = vertexData;
	m_IndexData = indexData;
	m_IndexCount = indexCount;

	m_VertexBufferHandle = bgfx::createVertexBuffer(m_VertexData, vertexLayout);
	m_IndexBufferHandle = bgfx::createIndexBuffer(m_IndexData);
}

void Mesh::unload()
{
	bgfx::destroy(m_VertexBufferHandle);
	bgfx::destroy(m_IndexBufferHandle);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	unload();
}
