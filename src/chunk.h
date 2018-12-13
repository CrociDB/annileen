#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <cstdlib>
#include <ctime>
#include <PerlinNoise.hpp>

#include "material.h"
#include "mesh.h"
#include "data.h"

#define CHUNK_WIDTH             16
#define CHUNK_HEIGHT            16
#define CHUNK_DEPTH             16
#define CHUNK_TOTAL_VOXELS      CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH

enum BlockType
{
    BlockEmpty = -1,
    BlockDirt = 0,
    BlockGrass,
    BlockSand
};

class Chunk
{
private:
    int m_WorldX;
    int m_WorldZ;
    
    siv::PerlinNoise* m_Noise;

    BlockType* m_Grid;

    Material* m_Material;
    Mesh* m_Mesh = nullptr;

    void generateMesh();
    float* generateMeshData(int* meshSize);

    bool gridEmpty(int x, int y, int z);

public:
    void setNoise(siv::PerlinNoise* noise) { m_Noise = noise; };
    void setMaterial(Material* material) { m_Material = material; }

    void generateGrid();

    
    Mesh* getMesh();

    Chunk(int wx, int wz);
    ~Chunk();
};

#endif