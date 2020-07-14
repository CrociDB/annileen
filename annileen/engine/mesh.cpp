#include "mesh.h"

using namespace annileen;

void Mesh::init(uint8_t* vertexData, uint16_t vertexDataSize, bgfx::VertexLayout vertexLayout, uint8_t* indices, uint16_t indicesCount)
{
	m_VertexData = vertexData;
	m_VertexDataSize = vertexDataSize;
	m_IndicesData = indices;
	m_IndicesDataSize = indicesCount;
}

void Mesh::unload()
{
	if (!m_Loaded) return;
}

Mesh::Mesh()
{
	m_Loaded = false;
}

Mesh::~Mesh()
{
	unload();
}
