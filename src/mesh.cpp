#include "mesh.h"

void Mesh::init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr, const uint32_t* indices, uint32_t indicesCount)
{
    m_VertexAttr = vertexAttr;

    m_VertexData = new GLfloat[verticesCount];
    m_DataSize = verticesCount;

    for (unsigned int i = 0; i < m_DataSize; i++)
    {
        m_VertexData[i] = vertexData[i];
    }

    m_UseVertexIndices = indicesCount > 0;
    if (m_UseVertexIndices)
    {
        m_VertexIndices = new GLuint[indicesCount];
        m_VertexIndicesCount = indicesCount;

        for (unsigned int i = 0; i < m_VertexIndicesCount; i++)
        {
            m_VertexIndices[i] = indices[i];
        }
    }

    m_VertexAttrCount = 1
        + (m_VertexAttr & VERTEX_COLOR)
        + ((m_VertexAttr & VERTEX_UV) >> 1)
        + ((m_VertexAttr & VERTEX_NORMAL) >> 2);

    m_VertexStride = sizeof(GLfloat) * (3
        + ((m_VertexAttr & VERTEX_COLOR) * 3)
        + (((m_VertexAttr & VERTEX_UV) >> 1) * 2)
        + (((m_VertexAttr & VERTEX_NORMAL) >> 2) * 3));

    m_VertexCount = m_DataSize / (m_VertexStride / sizeof(GLfloat));

    m_BuffersGenerated = false;
}

void Mesh::init(const float* vertexData, uint32_t verticesCount, uint8_t vertexAttr)
{
    init(vertexData, verticesCount, vertexAttr, NULL, 0);
}

void Mesh::setMaterial(Material* material)
{
    m_Material = material;
}

Material* Mesh::getMaterial()
{
    return m_Material;
}

void Mesh::genBuffers()
{
    if (m_BuffersGenerated)
    {
        return;
    }

    glGenVertexArrays(1, &m_VertexArrayObject);
    glGenBuffers(1, &m_VertexBufferObject);

    glBindVertexArray(m_VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_DataSize * sizeof(GLfloat), m_VertexData, GL_STATIC_DRAW);

    if (m_UseVertexIndices)
    {
        glGenBuffers(1, &m_ElementBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndicesCount * sizeof(GLuint), m_VertexIndices, GL_STATIC_DRAW);
    }

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

    if (m_VertexAttr & VERTEX_NORMAL)
    {
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, m_VertexStride, (GLvoid*)(offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
        offset += 3;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_BuffersGenerated = true;
}

void Mesh::bindAndDraw()
{
    glBindVertexArray(m_VertexArrayObject);

    if (m_UseVertexIndices)
    {
        glDrawElements(GL_TRIANGLES, m_VertexIndicesCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
    }

    glBindVertexArray(0);
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VertexArrayObject);
    glDeleteBuffers(1, &m_VertexBufferObject);
    if (m_UseVertexIndices)
        glDeleteBuffers(1, &m_ElementBufferObject);
}
