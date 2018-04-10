#include "mesh.h"

void Mesh::init(float* vertices, int verticesCount)
{
    m_Vertices = new float[verticesCount];
    memcpy(m_Vertices, vertices, verticesCount);
}

void Mesh::setMaterial(Material* material)
{
    m_Material = std::shared_ptr<Material>(material);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}
