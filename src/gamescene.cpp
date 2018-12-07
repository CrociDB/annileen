#include "gamescene.h"

void GameScene::buildMap()
{
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../shaders/fragment.frag");

    m_BlockMaterial = new Material();
    m_BlockMaterial->init(shader);
    m_BlockMaterial->addTexture("mainTex", Texture("../../assets/texture.png"));

    Chunk* testChunk = new Chunk(16, 6, 16);
    testChunk->setMaterial(m_BlockMaterial);
    testChunk->generateGrid();
    m_Chunks.push_back(testChunk);

    addMesh(testChunk->getMesh());
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    delete m_BlockMaterial;
}
