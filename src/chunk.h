#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <cstdlib>
#include <ctime>

#include <PerlinNoise.hpp>

#include "material.h"
#include "mesh.h"
#include "data.h"

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
    int m_Width;
    int m_Height;
    int m_Depth;

    siv::PerlinNoise* m_Noise;

    BlockType* m_Grid;

    Material* m_Material;
    Mesh* m_Mesh = nullptr;

    void generateMesh();
    float* generateMeshData(int* meshSize);

    bool gridEmpty(int x, int y, int z);

public:
    void setMaterial(Material* material) { m_Material = material; }

    void generateGrid();

    Mesh* getMesh();

    Chunk(int w, int h, int d);
    ~Chunk();
};

#endif