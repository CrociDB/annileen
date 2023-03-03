module;

#include <iostream>
#include <vector>
#include <memory>
#include <bgfx/bgfx.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

export module anni.mesh;

import anni.asset;

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

		Mesh() = default;
		~Mesh();
	};

	class MeshGroup : public AssetObject
	{
	public:
		std::vector<std::shared_ptr<Mesh>> m_Meshes;
		~MeshGroup();
	};
}

module :private;

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

	Mesh::~Mesh()
	{
		unload();

		// TODO: remove
		std::cout << "Mesh destroyed." << std::endl;
	}

	MeshGroup::~MeshGroup()
	{
		m_Meshes.clear();

		// TODO: remove
		std::cout << "MeshGroup destroyed." << std::endl;
	}
}
