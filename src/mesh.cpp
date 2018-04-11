#include "mesh.h"

void Mesh::init(float* vertices, int verticesCount)
{
    m_Vertices = new GLfloat[verticesCount];
    m_VerticesCount = verticesCount;
    for (int i = 0; i < m_VerticesCount; i++)
    {
        m_Vertices[i] = vertices[i];
    }
}

void Mesh::setMaterial(Material* material)
{
    m_Material = std::shared_ptr<Material>(material);
}

Material* Mesh::getMaterial()
{
    return m_Material.get();
}

void Mesh::genBuffers()
{
    glGenVertexArrays(1, &m_VertexArrayObject);
    glGenBuffers(1, &m_VertexBufferObject);

    glBindVertexArray(m_VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_VerticesCount * sizeof(GLfloat), m_Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::bindAndDraw()
{
    glBindVertexArray(m_VertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VertexArrayObject);
    glDeleteBuffers(1, &m_VertexBufferObject);
}
