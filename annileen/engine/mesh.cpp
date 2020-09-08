#include "mesh.h"

namespace annileen
{
	void Mesh::init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout, const bgfx::Memory* indexData)
	{
		m_HasIndices = (indexData != nullptr);

		m_VertexBufferHandle = bgfx::createVertexBuffer(vertexData, vertexLayout);
		if (m_HasIndices)
			m_IndexBufferHandle = bgfx::createIndexBuffer(indexData, BGFX_BUFFER_INDEX32);

		m_Loaded = true;
	}

	void Mesh::init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout)
	{
		init(vertexData, vertexLayout, nullptr);
	}

	void Mesh::unload()
	{
		if (!m_Loaded) return;

		bgfx::destroy(m_VertexBufferHandle);
		if (m_HasIndices)
		{
			bgfx::destroy(m_IndexBufferHandle);
		}
	}

	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
		unload();
	}

	MeshGroup::~MeshGroup()
	{
		for (auto& m : m_Meshes)
		{
			delete m;
		}
	}
}
