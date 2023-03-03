module;

#include <cstdlib>
#include <ctime>
#include <PerlinNoise.hpp>
#include <engine/forward_decl.h>
#include <tuple>
#include <iostream>
#include <time.h>
#include <glm.hpp>
#include <bgfx/bgfx.h>
#include "data.h"

export module chunk;

import anni.engine;
import anni.mesh;
import anni.allocators;
import anni.scenemanager;
import anni.scene;
import anni.scenenode;
import anni.model;
import anni.material;

using namespace annileen;

export enum BlockType : uint8_t
{
    BlockEmpty = 255,
    BlockDirt = 0,
    BlockGrass,
    BlockSand,
	BlockStone
};

export const int CHUNK_WIDTH = 16;
export const int CHUNK_HEIGHT = 80;
export const int CHUNK_DEPTH = 16;
export const int CHUNK_TOTAL_VOXELS = CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH;
export const int CHUNK_PERIOD = 6;
export const float CHUNK_OCTAVE = 35.8f;
export template<typename T> constexpr T GRID_AT(T X, T Y, T Z) 
{
    return  Z + Y * CHUNK_WIDTH + X * CHUNK_HEIGHT * CHUNK_DEPTH;
}

export class Chunk
{
private:
    int m_WorldX;
    int m_WorldZ;
    
    siv::PerlinNoise* m_Noise;

    BlockType* m_Grid;

    std::shared_ptr<Material> m_Material{ nullptr };
    std::shared_ptr<MeshGroup> m_MeshGroup{ nullptr };
    std::shared_ptr<Model> m_Model{ nullptr };
    std::shared_ptr<SceneNode> m_Node{ nullptr };

    void generateMesh();
    float* generateMeshData(int* meshSize);

    bool gridEmpty(int x, int y, int z);

public:
    void setNoise(siv::PerlinNoise* noise) { m_Noise = noise; };
    void setMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    void generateGrid();
    std::shared_ptr<SceneNode> getSceneNode(std::shared_ptr<Scene> scene);

    Chunk(int wx, int wz);
    ~Chunk();
};

void Chunk::generateMesh()
{
    m_MeshGroup = std::make_shared<annileen::MeshGroup>();

    int meshSize;
    float* meshData = generateMeshData(&meshSize);

    bgfx::VertexLayout vlayout;
    vlayout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .end();

    auto mesh = std::make_shared<annileen::Mesh>();
    mesh->init(bgfx::makeRef(meshData, meshSize * sizeof(float), annileen::Allocators::releaseMem), vlayout);
    m_MeshGroup->m_Meshes.push_back(mesh);
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
    srand(static_cast<unsigned int>(time(nullptr)));

    m_Grid = new BlockType[CHUNK_TOTAL_VOXELS];

    for (int i = 0; i < CHUNK_TOTAL_VOXELS; i++)
    {
        m_Grid[i] = BlockEmpty;
    }

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            double noise0 = m_Noise->accumulatedOctaveNoise2D_0_1(
                static_cast<double>((m_WorldX * CHUNK_WIDTH) + static_cast<double>(x)) / static_cast<double>(CHUNK_WIDTH * CHUNK_PERIOD) * 0.4,
                static_cast<double>((m_WorldZ * CHUNK_DEPTH) + static_cast<double>(z)) / static_cast<double>(CHUNK_DEPTH * CHUNK_PERIOD) * 0.4,
                1) * .8f + .2f;

            double noise = m_Noise->accumulatedOctaveNoise2D_0_1(
                static_cast<double>((m_WorldX * CHUNK_WIDTH) + static_cast<double>(x)) / static_cast<double>(CHUNK_WIDTH * CHUNK_PERIOD) * 1.0,
                static_cast<double>((m_WorldZ * CHUNK_DEPTH) + static_cast<double>(z)) / static_cast<double>(CHUNK_DEPTH * CHUNK_PERIOD) * 1.0,
                5);

            /*float noise0 = 0;
            float noise = 0;*/

            noise *= noise0;

            int base = (int)(.2f * CHUNK_HEIGHT);
            int sy = (int)(noise * (CHUNK_HEIGHT - base)) + base;

            if (sy < (.38f * CHUNK_HEIGHT))
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
            else if (sy > (.65f * CHUNK_HEIGHT))
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

std::shared_ptr<SceneNode> Chunk::getSceneNode(std::shared_ptr<Scene> scene)
{
    if (m_Node == nullptr)
    {
        m_Node = scene->createNode("Chunk");

        m_Model = annileen::SceneManager::getInstance()->addModule<annileen::Model>(scene, m_Node);
        m_Model->init(m_MeshGroup, m_Material);

        m_Node->getTransform().position(glm::vec3(
            m_WorldX * CHUNK_WIDTH,
            0,
            m_WorldZ * CHUNK_DEPTH));
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
    delete m_Grid;

    // TODO: remove
    std::cout << "Chunk destroyed." << std::endl;
}
