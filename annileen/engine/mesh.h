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
        bool m_Loaded;

        bgfx::VertexBufferHandle m_VertexBufferHandle;
        bgfx::IndexBufferHandle m_IndexBufferHandle;

        uint16_t m_VertexDataSize;
        uint8_t* m_VertexData;

        uint16_t m_IndicesDataSize;
        uint8_t* m_IndicesData;

        bgfx::VertexLayout m_VertexLayout;

    public:
        void init(uint8_t* vertexData, uint16_t vertexDataSize, bgfx::VertexLayout vertexLayout, uint8_t* indices, uint16_t indicesCount);
        //void init(RawMesh* rawMesh, bgfx::VertexLayout m_VertexLayout);

        void unload();

        Mesh();
        ~Mesh();
    };
}

