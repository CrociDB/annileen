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
    int vertices = sizeof(DATA_CUBE_VERTICES) / sizeof(float);
    int uvs = sizeof(DATA_CUBE_NORMALIZED_UVS) / sizeof(float);
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

                if (m_Grid[i] == BlockEmpty)
                { 
                    continue;
                }

                for (int f = 0; f < 6; f++)
                {
                    bool showFace = gridEmpty(x + DATA_CUBE_VALIDATIONS[f][0],
                        y + DATA_CUBE_VALIDATIONS[f][1],
                        z + DATA_CUBE_VALIDATIONS[f][2]);

                    if (showFace)
                    {
                        for (int j = 0; j < 6; j++)
                        {
                            int jv = j * 3;
                            int ju = j * 2;

                            // Vertices
                            data[data_i++] = DATA_CUBE_VERTICES[f][jv] + (float)x;
                            data[data_i++] = DATA_CUBE_VERTICES[f][jv + 1] + (float)y;
                            data[data_i++] = DATA_CUBE_VERTICES[f][jv + 2] + (float)z;

                            float uvx = (1.0f / (float)DATA_CUBE_TEX_SIZE[0]) * (float)DATA_CUBE_TILE[m_Grid[i]][0];
                            float uvy = (1.0f / (float)DATA_CUBE_TEX_SIZE[1]) * (float)DATA_CUBE_TILE[m_Grid[i]][1];

                            // UVs
                            data[data_i++] = (DATA_CUBE_NORMALIZED_UVS[f][ju] / DATA_CUBE_TEX_SIZE[0]) + uvx;
                            data[data_i++] = (DATA_CUBE_NORMALIZED_UVS[f][ju + 1] / DATA_CUBE_TEX_SIZE[1]) + uvy;
                        }
                    }
                }

            }
        }
    }

    (*meshSize) = data_size;
    return data;
}

bool Chunk::gridEmpty(int x, int y, int z)
{
    if (x >= m_Width || y >= m_Height || z >= m_Depth ||
        x < 0 || y < 0 || z < 0)
    {
        return true;
    }

    int i = GRID_AT(x, y, z);

    return m_Grid[i] == BlockEmpty;
}

void Chunk::generateGrid()
{
    srand(time(NULL));

    int total = m_Width * m_Depth * m_Height;

    m_Grid = new BlockType[total];
    for (int i = 0; i < total; i++)
    {
        m_Grid[i] = (BlockType)((rand() % 4) - 1);
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
