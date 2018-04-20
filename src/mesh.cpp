#include "mesh.h"

void Mesh::init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr, const uint32_t* indices, uint32_t indicesCount)
{
    m_VertexAttr = vertexAttr;

    m_VertexData = new GLfloat[verticesCount];
    m_VertexCount = verticesCount;

    for (int i = 0; i < m_VertexCount; i++)
    {
        m_VertexData[i] = vertexData[i];
    }

    m_VertexIndices = new GLuint[indicesCount];
    m_VertexIndicesCount = indicesCount;

    for (int i = 0; i < m_VertexIndicesCount; i++)
    {
        m_VertexIndices[i] = indices[i];
    }

    m_VertexAttrCount = 1
        + (m_VertexAttr & VERTEX_COLOR)
        + ((m_VertexAttr & VERTEX_UV) >> 1)
        + ((m_VertexAttr & VERTEX_NORMAL) >> 2);

    m_VertexStride = 3 * sizeof(GLfloat)
        + ((m_VertexAttr & VERTEX_COLOR) * 3 * sizeof(GLfloat))
        + (((m_VertexAttr & VERTEX_UV) >> 1) * 2 * sizeof(GLfloat))
        + (((m_VertexAttr & VERTEX_NORMAL) >> 2) * 3 * sizeof(GLfloat));
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
    glGenBuffers(1, &m_ElementBufferObject);

    glBindVertexArray(m_VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(GLfloat), m_VertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndicesCount * sizeof(GLuint), m_VertexIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_VertexStride, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    int offset = 3;
    
    if (m_VertexAttr & VERTEX_COLOR)
    {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_VertexStride, (GLvoid*)(offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        offset += 3;
    }

    if (m_VertexAttr & VERTEX_UV)
    {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, m_VertexStride, (GLvoid*)(offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        offset += 2;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::bindAndDraw()
{
    glBindVertexArray(m_VertexArrayObject);
    glDrawElements(GL_TRIANGLES, m_VertexIndicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VertexArrayObject);
    glDeleteBuffers(1, &m_VertexBufferObject);
    glDeleteBuffers(1, &m_ElementBufferObject);
}
