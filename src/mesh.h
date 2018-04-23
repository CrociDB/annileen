#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "material.h"

// Mesh data
#define VERTEX_COLOR    1
#define VERTEX_UV       1 << 1
#define VERTEX_NORMAL   1 << 2

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

    glm::mat4 transform;

public:
    glm::vec3 scale;
    glm::vec3 position;
    glm::quat rotation;

    void init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr, const uint32_t* indices, uint32_t indicesCount);
    void setMaterial(Material* material);
    Material* getMaterial();

    void genBuffers();
    glm::mat4 getUpdatedTransformMatrix();
    void bindAndDraw();

    Mesh();
    ~Mesh();
};

#endif
