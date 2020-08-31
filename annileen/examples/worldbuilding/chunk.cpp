#include "chunk.h"
#include "engine/engine.h"

#define GRID_AT(X, Y, Z)        Z + Y * CHUNK_WIDTH + X * CHUNK_HEIGHT * CHUNK_DEPTH

void Chunk::generateMesh()
{
    if (m_Mesh != nullptr)
    {
        delete m_Node;
        delete m_Mesh;
    }

    m_Mesh = new annileen::Mesh();

    int meshSize;
    int indexSize;
    float* meshData = generateMeshData(&meshSize);

    bgfx::VertexLayout vlayout;
    vlayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .end();


    m_Mesh->init(bgfx::makeRef(meshData, meshSize * sizeof(float), Engine::releaseMem), vlayout);

    m_Model = std::make_shared<Model>();
    m_Model->init(m_Mesh, m_Material);
}

float* Chunk::generateMeshData(int* meshSize)
{
    size_t ds = 0;
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
                        ds += 6 * (3 + 2 + 3);
                    }
                }

            }
        }
    }

    float* data = new float[ds + 1];
    int data_i = 0;
    int data_index = 0;

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

                            float uvx = (1.0f / (float)DATA_CUBE_TEX_SIZE[0]) * (float)DATA_CUBE_TILE[m_Grid[i]][f][0];
                            float uvy = (1.0f / (float)DATA_CUBE_TEX_SIZE[1]) * (float)DATA_CUBE_TILE[m_Grid[i]][f][1];

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

    (*meshSize) = (data_i + 1);
    return data;
}

bool Chunk::gridEmpty(int x, int y, int z)
{
    if (x >= CHUNK_WIDTH || y >= CHUNK_HEIGHT || z >= CHUNK_DEPTH ||
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
			float noise0 = m_Noise->accumulatedOctaveNoise2D_0_1(
				(float)((m_WorldX * CHUNK_WIDTH) + (float)x) / (float)(CHUNK_WIDTH * CHUNK_PERIOD) * 0.3,
				(float)((m_WorldZ * CHUNK_DEPTH) + (float)z) / (float)(CHUNK_DEPTH * CHUNK_PERIOD) * 0.3,
				1) * .8f + .2f;

            float noise = m_Noise->accumulatedOctaveNoise2D_0_1(
                (float)((m_WorldX * CHUNK_WIDTH) + (float)x) / (float)(CHUNK_WIDTH * CHUNK_PERIOD) * 1.0,
                (float)((m_WorldZ * CHUNK_DEPTH) + (float)z) / (float)(CHUNK_DEPTH * CHUNK_PERIOD) * 1.0,
                5);

            /*float noise0 = 0;
            float noise = 0;*/

			noise *= noise0;

            int base = (int)(.3f * CHUNK_HEIGHT);
            int sy = (int)(noise * (CHUNK_HEIGHT - base)) + base;

            if (sy < (.48f * CHUNK_HEIGHT))
            {
                for (int y = sy; y >= 0; y--)
                {
                    int i = GRID_AT(x, y, z);
					if (y < sy - 7)
						m_Grid[i] = BlockStone;
					else
						m_Grid[i] = BlockSand;
                }
            }
			else if (sy > (.78f * CHUNK_HEIGHT))
			{
				for (int y = sy; y >= 0; y--)
				{
					int i = GRID_AT(x, y, z);
					m_Grid[i] = BlockStone;
				}
			}
            else
            {
                int i = GRID_AT(x, sy, z);
				m_Grid[i] = BlockGrass;

                for (int y = sy - 1; y >= 0; y--)
                {
                    int i = GRID_AT(x, y, z);
					if (y < sy - 7)
						m_Grid[i] = BlockStone;
					else
						m_Grid[i] = BlockDirt;
                }
            }
        }
    }

    generateMesh();
}

Mesh* Chunk::getMesh()
{
    return m_Mesh;
}

SceneNodePtr Chunk::getSceneNode()
{
    if (m_Node == nullptr)
    {
        m_Node = Engine::getInstance()->getScene()->createNode();
        m_Node->setModel(m_Model);

        m_Node->getTransform().position = glm::vec3(
            m_WorldX * CHUNK_WIDTH,
            0,
            m_WorldZ * CHUNK_DEPTH);
    }

    return m_Node;
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
