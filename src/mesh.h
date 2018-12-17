#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "material.h"
#include "transform.h"

// Mesh data
#define VERTEX_COLOR    1
#define VERTEX_UV       1 << 1
#define VERTEX_NORMAL   1 << 2

class Mesh
{
private:
    bool m_BuffersGenerated;
    uint32_t m_VertexCount;
    uint32_t m_VertexIndicesCount;
    GLfloat* m_VertexData;
    size_t m_DataSize;
    GLuint* m_VertexIndices;
    bool m_UseVertexIndices;
    Material* m_Material;

    GLuint m_VertexArrayObject;
    GLuint m_VertexBufferObject;
    GLuint m_ElementBufferObject;

    uint8_t m_VertexAttr;
    uint8_t m_VertexAttrCount;
    uint16_t m_VertexStride;

public:
    Transform transform;

    void init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr, const uint32_t* indices, uint32_t indicesCount);
    void init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr);
    void setMaterial(Material* material);
    Material* getMaterial();

    void genBuffers();
    void bindAndDraw();

    Mesh();
    ~Mesh();
};

#endif
