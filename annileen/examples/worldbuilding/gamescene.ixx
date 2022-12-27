module;

#include <vector>
#include <deque>
#include <unordered_map>
#include <PerlinNoise.hpp>
#include <cstdlib>
#include <ctime>

export module gamescene;

import scene;
import material;
import chunk;

export const int GAME_CHUNK_RADIUS = 4;
export const int GAME_CHUNK_MAX = (GAME_CHUNK_RADIUS * GAME_CHUNK_RADIUS * 4) + (GAME_CHUNK_RADIUS * 10);

using namespace annileen;

export class GameScene : public Scene
{
private:
    std::shared_ptr<Material> m_BlockMaterial;
	std::vector<Chunk*> m_Chunks;
	std::deque<uint64_t> m_ChunksToCreate;
    std::unordered_map<uint64_t, Chunk*> m_AvailableChunks;

    siv::PerlinNoise* m_Noise;

    void createChunkAt(int x, int z);
    void removeFarthestChunk();

    void addChunk(Chunk* chunk);
    void removeChunk(Chunk* chunk);
public:
    void buildMap();

    void start() override;
    void update() override;

    GameScene();
    ~GameScene();

};
