#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <vector>
#include <deque>
#include <unordered_map>
#include <PerlinNoise.hpp>
#include <cstdlib>
#include <ctime>

#include "scene.h"
#include "chunk.h"

#define GAME_CHUNK_RADIUS           8
#define GAME_CHUNK_MAX              (GAME_CHUNK_RADIUS * GAME_CHUNK_RADIUS * 4) + (GAME_CHUNK_RADIUS * 10)

class GameScene : public Scene
{
private:
    Material* m_BlockMaterial;
	std::vector<Chunk*> m_Chunks;
	std::deque<uint64_t> m_ChunksToCreate;
    std::unordered_map<uint64_t, Chunk*> m_AvailableChunks;

    siv::PerlinNoise* m_Noise;

    void createChunkAt(int x, int z);
    void removeFarthestChunk();
public:
    void buildMap();

    void start() override;
    void update() override;

    GameScene();
    ~GameScene();

};

#endif