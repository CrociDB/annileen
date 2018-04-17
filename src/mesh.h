#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>

#include "material.h"

// Mesh data
#define VERTEX_COLOR    0b00000001
#define VERTEX_UV       0b00000010
#define VERTEX_NORMAL   0b00000100

class Mesh
{
private:
    uint32_t m_VertexCount;
    uint32_t m_VertexIndicesCount;
    GLfloat* m_VertexData;
    GLuint* m_VertexIndices;
    std::shared_ptr<Material> m_Material;

    GLuint m_VertexArrayObject;
    GLuint m_VertexBufferObject;
    GLuint m_ElementBufferObject;

    uint8_t m_VertexAttr;
    uint8_t m_VertexAttrCount;
    uint16_t m_VertexStride;

public:
    void init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr, const uint32_t* indices, uint32_t indicesCount);
    void setMaterial(Material* material);
    Material* getMaterial();

    void genBuffers();
    void bindAndDraw();

    Mesh();
    ~Mesh();
};

#endif
