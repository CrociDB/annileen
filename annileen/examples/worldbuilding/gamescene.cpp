#include "gamescene.h"

#include <engine/engine.h>
#include <engine/skybox.h>
#include <engine/serviceprovider.h>
#include <engine/shaderpass.h>
#include <engine/text/text.h>

using namespace annileen;

void GameScene::buildMap()
{
    auto texture = ServiceProvider::getAssetManager()->loadTexture("blocks.png");

    Shader* shader = nullptr;

    if (ServiceProvider::getSettings()->shadows.enabled)
    {
        shader = ServiceProvider::getAssetManager()->loadShader("voxel_shadow.vs", "voxel_shadow.fs");
    }
    else
    {
        shader = ServiceProvider::getAssetManager()->loadShader("voxel_noshadow.vs", "voxel_noshadow.fs");
    }

    std::shared_ptr<ShaderPass> shaderPass = std::make_shared<ShaderPass>();
    shaderPass->init(shader);
    shaderPass->setState(BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | UINT64_C(0));

    m_BlockMaterial = std::make_shared<Material>();
    m_BlockMaterial->addTexture("s_mainTex", texture, 0);
    m_BlockMaterial->addShaderPass(shaderPass);

    fog.color = glm::vec3(0.823f, 0.705f, 0.513f);
    fog.distance = 150.0f;
    fog.enabled = 1.0f;
    fog.power = 1.3f;

    SceneNodePtr lightNode = createNode("Light");
    Light* light = lightNode->addModule<Light>();

    light->color = glm::vec3(1.0f, 1.0f, .8f);
    light->type = LightType::Directional;
    light->intensity = 0.8f;
    light->getTransform().rotate(glm::vec3(-40.0f, 0.0f, -40.0f));

    SceneNodePtr cameraNode = createNode("Camera");

    Camera* camera = cameraNode->addModule<Camera>();
    camera->fieldOfView = 60.0f;
    camera->nearClip = 0.1f;
    camera->farClip = 300.0f;

    SceneNodePtr textNode = createNode("Text");
    Text* text = textNode->addModule<Text>();

    auto cubemap = ServiceProvider::getAssetManager()->loadCubemap("skybox.toml");
    auto skybox = new Skybox(cubemap);
    this->setSkybox(skybox);

    getCamera()->clearType = CameraClearType::CameraClearSkybox;

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
    auto cameraPos = getCamera()->getTransform().position;

    bool k = false;
    uint64_t ikill;
    float dist = 0.0f;

    for (const auto& c : m_AvailableChunks)
    {
        if (c.second == nullptr)
        {
            continue;
        }

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
        m_AvailableChunks.erase(ikill);
        removeChunk(chunk);
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
    
    getCamera()->getTransform().position = glm::vec3(0.0f, 70.0f, 0.0f);
}

void GameScene::update()
{
    // know exactly what chunks to generate and render
    // TODO: the render/occlusion part will have to be moved over to a better place later
    //clearChunks();

    auto cameraPos = getCamera()->getTransform().position;
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

    for (int x = cx - GAME_CHUNK_RADIUS; x < cx + GAME_CHUNK_RADIUS; x++)
    {
        for (int z = cz - GAME_CHUNK_RADIUS; z < cz + GAME_CHUNK_RADIUS; z++)
        {
			uint64_t ca = (uint32_t)x | (((uint64_t)z) << 32);
            if (m_AvailableChunks.count(ca) == 0)
            {
                if (std::find(m_ChunksToCreate.begin(), m_ChunksToCreate.end(), ca) == m_ChunksToCreate.end())
				    m_ChunksToCreate.push_back(ca);
            }

            if (m_AvailableChunks.count(ca) > 0)
            {
                auto chunk = m_AvailableChunks.at(ca);
                addChunk(chunk);
            }
        }
    }
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}
