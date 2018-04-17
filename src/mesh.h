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
    uint64_t m_VertexSize;
    GLfloat* m_VertexData;
    std::shared_ptr<Material> m_Material;

    GLuint m_VertexArrayObject;
    GLuint m_VertexBufferObject;

    uint8_t m_VertexAttr;
    uint8_t m_VertexAttrCount;
    uint16_t m_VertexStride;

public:
    void init(const float* vertexData, int verticesCount, uint8_t vertexAttr);
    void setMaterial(Material* material);
    Material* getMaterial();

    void genBuffers();
    void bindAndDraw();

    Mesh();
    ~Mesh();
};

#endif
