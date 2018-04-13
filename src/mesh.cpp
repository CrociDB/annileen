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

void Mesh::setVertexColors(float* vertexColor)
{
    m_AttrVertexColor = true;
    m_VertexColors = new GLfloat[m_VerticesCount];
    for (int i = 0; i < m_VerticesCount; i++)
    {
        m_VertexColors[i] = vertexColor[i];
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

int Mesh::bufferAllData()
{
    int params = 1 + m_AttrVertexColor;
    int elem = m_VerticesCount * params;
    GLfloat* data = new GLfloat[elem];

    int c = 0;
    int i, j;
    for (i = 0; i < m_VerticesCount; i++)
    {
        data[c++] = m_Vertices[i];

        if ((i+1) % 3 == 0)
        {
            // Color
            if (m_AttrVertexColor)
                for (j = 0; j < 3; j++) data[c++] = m_VertexColors[(i-2) + j];
        }
    }

    glBufferData(GL_ARRAY_BUFFER, c * sizeof(GLfloat), data, GL_STATIC_DRAW);

    return params * 3;
}

void Mesh::genBuffers()
{
    glGenVertexArrays(1, &m_VertexArrayObject);
    glGenBuffers(1, &m_VertexBufferObject);

    glBindVertexArray(m_VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
    int p = bufferAllData();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, p * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    
    if (m_AttrVertexColor)
    {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, p * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }

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
    m_AttrVertexColor = false;
}


Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_VertexArrayObject);
    glDeleteBuffers(1, &m_VertexBufferObject);
}
