module;

#include <cstdlib>
#include <ctime>
#include <PerlinNoise.hpp>
#include <engine/forward_decl.h>

export module chunk;

import material;
import model;
import scenenode;
import mesh;

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

    std::shared_ptr<Material> m_Material;
    MeshGroup* m_MeshGroup = nullptr;
    ModelPtr m_Model = nullptr;
    SceneNodePtr m_Node = nullptr;

    void generateMesh();
    float* generateMeshData(int* meshSize);

    bool gridEmpty(int x, int y, int z);

public:
    void setNoise(siv::PerlinNoise* noise) { m_Noise = noise; };
    void setMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    void generateGrid();
    SceneNodePtr getSceneNode();

    Chunk(int wx, int wz);
    ~Chunk();
};
