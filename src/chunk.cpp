#include "chunk.h"

#define GRID_AT(X, Y, Z)        Z + Y * m_Width + X * m_Height * m_Depth

void Chunk::generateMesh()
{
    if (m_Mesh != nullptr)
    {
        delete m_Mesh;
    }

    m_Mesh = new Mesh();
    m_Mesh->setMaterial(m_Material);


    int meshSize;
    float* meshData = generateMeshData(&meshSize);

    m_Mesh->init(meshData, meshSize, VERTEX_UV);
}

float* Chunk::generateMeshData(int* meshSize)
{
    int voxels = m_Width * m_Depth * m_Height;
    int vertices = sizeof(cube_vertices) / sizeof(float);
    int uvs = sizeof(cube_uvs) / sizeof(float);
    int data_size = voxels * (vertices + uvs);

    float* data = new float[data_size];
    int data_i = 0;

    for (int x = 0; x < m_Width; x++)
    {
        for (int y = 0; y < m_Height; y++)
        {
            for (int z = 0; z < m_Depth; z++)
            {
                int i = GRID_AT(x, y, z);

                for (int j = 0; j < vertices / 3; j++)
                {
                    int jv = j * 3;
                    int ju = j * 2;

                    // Vertices
                    data[data_i++] = cube_vertices[jv]      + (float)x;
                    data[data_i++] = cube_vertices[jv + 1]  + (float)y;
                    data[data_i++] = cube_vertices[jv + 2]  + (float)z;

                    // UVs
                    data[data_i++] = (cube_uvs[ju] / 4) + (0.333333 * 1);
                    data[data_i++] = (cube_uvs[ju + 1] / 2) + (0.5 * 0);
                }

                /*if (m_Grid[i] == BlockGrass)
                { 

                }*/
            }
        }
    }

    (*meshSize) = data_size;
    return data;
}

void Chunk::generateGrid()
{
    int total = m_Width * m_Depth * m_Height;

    m_Grid = new BlockType[total];
    for (int i = 0; i < total; i++)
    {
        m_Grid[i] = BlockSand;
    }

    generateMesh();
}

Mesh* Chunk::getMesh()
{
    return m_Mesh;
}

Chunk::Chunk(int w, int h, int d)
{
    m_Width = w;
    m_Height = h;
    m_Depth = d;
}

Chunk::~Chunk()
{
    delete m_Mesh;
    delete m_Grid;
}
