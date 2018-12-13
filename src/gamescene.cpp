#include "gamescene.h"

void GameScene::buildMap()
{
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../shaders/fragment.frag");

    m_BlockMaterial = new Material();
    m_BlockMaterial->init(shader);
    m_BlockMaterial->addTexture("mainTex", Texture("../../assets/texture.png"));
    m_BlockMaterial->loadTextures();

    m_Noise = new siv::PerlinNoise(time(NULL));

    for (int x = 0; x < 5; x++)
    {
        for (int z = 0; z < 5; z++)
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
