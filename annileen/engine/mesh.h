#pragma once

#include <iostream>
#include <memory>

#include <bgfx/bgfx.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace annileen
{
    class Mesh
    {
    private:
        bgfx::VertexBufferHandle m_VertexBufferHandle;
        bgfx::IndexBufferHandle m_IndexBufferHandle;

        const bgfx::Memory* m_VertexData;
        const bgfx::Memory* m_IndexData;

        size_t m_IndexCount;

        bgfx::VertexLayout m_VertexLayout;

    public:
        void init(const bgfx::Memory* vertexData, bgfx::VertexLayout vertexLayout, const bgfx::Memory* indexData, uint16_t indexCount);
        //void init(RawMesh* rawMesh, bgfx::VertexLayout m_VertexLayout);

        void unload();

        Mesh();
        ~Mesh();
    };
}

