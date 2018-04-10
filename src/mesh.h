#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <memory>

#include "material.h"

class Mesh
{
private:
    int m_VerticesCount;
    float* m_Vertices;
    std::shared_ptr<Material> m_Material;

public:
    void init(float* vertices, int verticesCount);
    void setMaterial(Material* material);

    Mesh();
    ~Mesh();
};

#endif
