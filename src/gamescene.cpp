#include "gamescene.h"

void GameScene::buildMap()
{
    Shader* shader = new Shader();
    shader->load("../../shaders/standard.vert", "../../shaders/standard.frag");

    m_BlockMaterial = new Material();
    m_BlockMaterial->init(shader);
    m_BlockMaterial->addTexture("mainTex", Texture("../../assets/texture.png"));
    m_BlockMaterial->loadTextures();

    fog.color = glm::vec3(0.823f, 0.705f, 0.513f);
    fog.distance = 80.0f;
    fog.enabled = 1.0f;
    fog.power = 1.2f;

    m_Noise = new siv::PerlinNoise(time(NULL));
}

void GameScene::createChunkAt(int x, int z)
{
    Chunk* chunk = new Chunk(x, z);
    chunk->setMaterial(m_BlockMaterial);
    chunk->setNoise(m_Noise);
    chunk->generateGrid();
    int i = GAME_CHUNK_GRID_AT(x, z);
    m_AvailableChunks.insert(std::pair<int, Chunk*>(i, chunk));
}

void GameScene::update()
{
    // know exactly what chunks to generate and render
    // TODO: the render/occlusion part will have to be moved over to a better place later
    clearMeshList();

    auto cameraPos = getCamera()->transform.position;
    int cx = static_cast<int>(cameraPos.x / CHUNK_WIDTH);
    int cz = static_cast<int>(cameraPos.z / CHUNK_DEPTH);

    for (int x = cx - GAME_CHUNK_RADIUS; x < cx + GAME_CHUNK_RADIUS; x++)
    {
        for (int z = cz - GAME_CHUNK_RADIUS; z < cz + GAME_CHUNK_RADIUS; z++)
        {
            int i = GAME_CHUNK_GRID_AT(x, z);

            if (m_AvailableChunks.count(i) == 0)
            {
                createChunkAt(x, z);

            }

            auto chunk = m_AvailableChunks.at(i);
            addMesh(chunk->getMesh());
        }
    }
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    delete m_BlockMaterial;
}
