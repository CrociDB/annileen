#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <vector>
#include <deque>
#include <unordered_map>
#include <PerlinNoise.hpp>
#include <cstdlib>
#include <ctime>

#include "engine/scene.h"

#include "chunk.h"

#define GAME_CHUNK_RADIUS           3
#define GAME_CHUNK_MAX              (GAME_CHUNK_RADIUS * GAME_CHUNK_RADIUS * 4) + (GAME_CHUNK_RADIUS * 10)

using namespace annileen;

// Example cube
struct PosColorVertex
{
    float pos[3];
    uint8_t col;
};
static PosColorVertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, (uint8_t)0xff000000 },
    { 1.0f,  1.0f,  1.0f, (uint8_t)0xff0000ff },
    {-1.0f, -1.0f,  1.0f, (uint8_t)0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, (uint8_t)0xff00ffff },
    {-1.0f,  1.0f, -1.0f, (uint8_t)0xffff0000 },
    { 1.0f,  1.0f, -1.0f, (uint8_t)0xffff00ff },
    {-1.0f, -1.0f, -1.0f, (uint8_t)0xffffff00 },
    { 1.0f, -1.0f, -1.0f, (uint8_t)0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
    2, 1, 0, // 0
    2, 3, 1,
    5, 6, 4, // 2
    7, 6, 5,
    4, 2, 0, // 4
    6, 2, 4,
    3, 5, 1, // 6
    3, 7, 5,
    1, 4, 0, // 8
    1, 5, 4,
    6, 3, 2, // 10
    7, 3, 6,
};

class GameScene : public Scene
{
private:
    std::shared_ptr<Material> m_BlockMaterial;
	std::vector<Chunk*> m_Chunks;
	std::deque<uint64_t> m_ChunksToCreate;
    std::unordered_map<uint64_t, Chunk*> m_AvailableChunks;

    siv::PerlinNoise* m_Noise;

    void createChunkAt(int x, int z);
    void removeFarthestChunk();

    void clearChunks();
    void addChunk(Chunk* chunk);
    void removeChunk(Chunk* chunk);
public:
    void buildMap();

    void start() override;
    void update() override;

    GameScene();
    ~GameScene();

};

#endif
