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

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            Chunk* chunk = new Chunk(x, z);
            chunk->setMaterial(m_BlockMaterial);
            chunk->setNoise(m_Noise);
            chunk->generateGrid();
            m_Chunks.push_back(chunk);
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
