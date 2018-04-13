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
    GLfloat* m_VertexColors;
    std::shared_ptr<Material> m_Material;

    GLuint m_VertexArrayObject;
    GLuint m_VertexBufferObject;

    bool m_AttrVertexColor;

    int bufferAllData();

public:
    void init(float* vertices, int verticesCount);
    void setVertexColors(float* vertexColor);
    void setMaterial(Material* material);
    Material* getMaterial();

    void genBuffers();
    void bindAndDraw();

    Mesh();
    ~Mesh();
};

#endif
