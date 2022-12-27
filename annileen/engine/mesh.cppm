module;

#include <iostream>
#include <vector>
#include <memory>
#include <bgfx/bgfx.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

export module mesh;

import asset;

export namespace annileen
{
	class Mesh
	{
	private:
		bool m_Loaded = false;
		bgfx::VertexBufferHandle m_VertexBufferHandle;
		bgfx::IndexBufferHandle m_IndexBufferHandle;

		bgfx::VertexLayout m_VertexLayout;

		bool m_HasIndices;

	public:
		void init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout, const bgfx::Memory* indexData);
		void init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout);

		bool hasIndices() { return m_HasIndices; }

		bgfx::VertexBufferHandle getVertexBuffer() { return m_VertexBufferHandle; }
		bgfx::IndexBufferHandle getIndexBuffer() { return m_IndexBufferHandle; }

		void unload();

		Mesh();
		~Mesh();
	};

	class MeshGroup : public AssetObject
	{
	public:
		std::vector<Mesh*> m_Meshes;
		~MeshGroup();
	};
}

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
