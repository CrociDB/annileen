#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>

#include "material.h"

class Mesh
{
private:
    int m_VerticesCount;
    GLfloat* m_Vertices;
    std::shared_ptr<Material> m_Material;

    GLuint m_VertexArrayObject;
    GLuint m_VertexBufferObject;

public:
    void init(float* vertices, int verticesCount);
    void setMaterial(Material* material);
    Material* getMaterial();

    void genBuffers();
    void bindAndDraw();

    Mesh();
    ~Mesh();
};

#endif
