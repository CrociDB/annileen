#include "gamescene.h"

#include "engine/engine.h"

using namespace annileen;

void GameScene::buildMap()
{
    auto texture = Engine::getInstance()->getAssetManager()->loadTexture("blocks.png");
    auto shader = Engine::getInstance()->getAssetManager()->loadShader("voxel.vs", "voxel.fs");

    m_BlockMaterial = std::make_shared<Material>();
    m_BlockMaterial->init(shader);
    m_BlockMaterial->addTexture("s_mainTex", texture);

    fog.color = glm::vec3(0.823f, 0.705f, 0.513f);
    fog.distance = 150.0f;
    fog.enabled = 1.0f;
    fog.power = 1.3f;

    m_Noise = new siv::PerlinNoise(std::random_device{});
}

void GameScene::createChunkAt(int x, int z)
{
    Chunk* chunk = new Chunk(x, z);
    chunk->setMaterial(m_BlockMaterial);
    chunk->setNoise(m_Noise);
    chunk->generateGrid();
	m_AvailableChunks.insert(std::pair<uint64_t, Chunk*>((uint32_t)x | (((uint64_t)z) << 32), chunk));
}

void GameScene::removeFarthestChunk()
{
    auto cameraPos = getCamera()->transform.position;

    bool k = false;
    uint64_t ikill;
    float dist = 0.0f;

    for (const auto& c : m_AvailableChunks)
    {
        auto d = glm::abs(glm::length(cameraPos - c.second->getSceneNode()->getTransform().position));
        if (dist < d)
        {
            k = true;
            dist = d;
            ikill = c.first;
        }
    }

    if (k)
    {
        auto chunk = m_AvailableChunks.at(ikill);
        removeChunk(chunk);
        m_AvailableChunks.erase(ikill);
        delete chunk;
    }
}

void GameScene::clearChunks()
{
    clearNodeList();
}

void GameScene::addChunk(Chunk* chunk)
{
    // Get it, so it gets built
    chunk->getSceneNode();
}

void GameScene::removeChunk(Chunk* chunk)
{
    removeNode(chunk->getSceneNode());
}

void GameScene::start()
{
    buildMap();
    
    getCamera()->transform.position = glm::vec3(0.0f, 40.0f, 0.0f);

    Light* light = new Light();
    light->color = glm::vec3(1.0f, 1.0f, .8f);
    light->type = LightDirectional;
    light->intensity = 0.8f;
    light->transform.rotate(glm::vec3(-40.0f, 0.0f, 0.0f));
    
    this->addLight(light);
}

void GameScene::update()
{
    // know exactly what chunks to generate and render
    // TODO: the render/occlusion part will have to be moved over to a better place later
    //clearChunks();

    auto cameraPos = getCamera()->transform.position;
    int cx = static_cast<int>(cameraPos.x / CHUNK_WIDTH);
    int cz = static_cast<int>(cameraPos.z / CHUNK_DEPTH);

	// Create chunk from list
	if (m_ChunksToCreate.size() > 0)
	{
		auto chunk_addr = m_ChunksToCreate.front();
		createChunkAt((int32_t)(chunk_addr & 0xFFFFFFFF), (int32_t)(((int64_t)(chunk_addr) >> 32) & 0xFFFFFFFF));
		m_ChunksToCreate.pop_front();
	}

	// Destroy chunk if needed
    if (m_AvailableChunks.size() > GAME_CHUNK_MAX)
    {
        // TODO: need to fix this removing routine
        removeFarthestChunk();
    }

    /*for (int x = cx - GAME_CHUNK_RADIUS; x < cx + GAME_CHUNK_RADIUS; x++)
    {
        for (int z = cz - GAME_CHUNK_RADIUS; z < cz + GAME_CHUNK_RADIUS; z++)
        {*/
			uint64_t ca = (uint32_t)cx | (((uint64_t)cz) << 32);
            if (m_AvailableChunks.count(ca) == 0 && std::find(m_ChunksToCreate.begin(), m_ChunksToCreate.end(), ca) == m_ChunksToCreate.end())
            {
				m_ChunksToCreate.push_back(ca);
            }

            if (m_AvailableChunks.count(ca) > 0)
            {
                auto chunk = m_AvailableChunks.at(ca);
                addChunk(chunk);
            }
     /*   }
    }*/
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}
