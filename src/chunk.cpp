#include "chunk.h"

#define GRID_AT(X, Y, Z)        Z + Y * CHUNK_WIDTH + X * CHUNK_HEIGHT * CHUNK_DEPTH

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

    m_Mesh->init(meshData, meshSize, VERTEX_UV | VERTEX_NORMAL);
    m_Mesh->transform.position = glm::vec3(
        m_WorldX * CHUNK_WIDTH,
        0,
        m_WorldZ * CHUNK_DEPTH);
}

float* Chunk::generateMeshData(int* meshSize)
{
    int vertices = sizeof(DATA_CUBE_VERTICES) / sizeof(float);
    int uvs = sizeof(DATA_CUBE_NORMALIZED_UVS) / sizeof(float);
    int data_size = CHUNK_TOTAL_VOXELS * (vertices + uvs);

    float* data = new float[data_size];
    int data_i = 0;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_DEPTH; z++)
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

                            // Normals
                            data[data_i++] = DATA_CUBE_NORMALS[f][jv];
                            data[data_i++] = DATA_CUBE_NORMALS[f][jv + 1];
                            data[data_i++] = DATA_CUBE_NORMALS[f][jv + 2];
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
    if (x >= CHUNK_DEPTH || y >= CHUNK_DEPTH || z >= CHUNK_DEPTH ||
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

    m_Grid = new BlockType[CHUNK_TOTAL_VOXELS];

    for (int i = 0; i < CHUNK_TOTAL_VOXELS; i++)
    {
        m_Grid[i] = BlockEmpty;
    }

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            float noise = m_Noise->octaveNoise0_1(
                (float)((m_WorldX * CHUNK_WIDTH) + (float)x) / (float)(CHUNK_WIDTH * CHUNK_PERIOD),
                (float)((m_WorldZ * CHUNK_DEPTH) + (float)z) / (float)(CHUNK_DEPTH * CHUNK_PERIOD),
                CHUNK_OCTAVE);

            int sy = (int)(noise * CHUNK_HEIGHT);

            for (int y = sy; y >= 0; y--)
            {
                int i = GRID_AT(x, y, z);
                //m_Grid[i] = (BlockType)(rand() % 3);
                m_Grid[i] = BlockDirt;
            }
        }
    }

    generateMesh();
}

Mesh* Chunk::getMesh()
{
    return m_Mesh;
}

Chunk::Chunk(int wx, int wz)
{
    m_WorldX = wx;
    m_WorldZ = wz;
}

Chunk::~Chunk()
{
    delete m_Mesh;
    delete m_Grid;
}
