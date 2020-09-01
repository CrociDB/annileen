#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <bgfx/bgfx.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "asset.h"

namespace annileen
{
    class Mesh
    {
    private:
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

    class MeshGroup : AssetObject
    {
    public:
        std::vector<Mesh> m_Meshes;
    };
}

