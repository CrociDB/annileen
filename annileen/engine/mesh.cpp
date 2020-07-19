#include "mesh.h"

using namespace annileen;

void Mesh::init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout, const bgfx::Memory* indexData, uint16_t indexCount)
{
	m_VertexData = vertexData;
	m_IndexData = indexData;
	m_IndexCount = indexCount;
	m_HasIndices = (indexData != nullptr);

	m_VertexBufferHandle = bgfx::createVertexBuffer(m_VertexData, vertexLayout);
	if (m_HasIndices)
		m_IndexBufferHandle = bgfx::createIndexBuffer(m_IndexData);
}

void Mesh::init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout)
{
	init(vertexData, vertexLayout, nullptr, 0);
}

void Mesh::unload()
{
	bgfx::destroy(m_VertexBufferHandle);
	if (m_HasIndices)
		bgfx::destroy(m_IndexBufferHandle);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	unload();
}
