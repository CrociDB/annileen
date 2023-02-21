module;

#include <vector>
#include <deque>
#include <unordered_map>
#include <PerlinNoise.hpp>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <memory>
#include <bgfx/bgfx.h>
#include <glm.hpp>
#include <PerlinNoise.hpp>

export module gamescene;

import engine;
import skybox;
import serviceprovider;
import shaderpass;
import text;
import scenenode;
import light;
import camera;
import scene;
import material;
import chunk;
import scenemanager;

using namespace annileen;

export const int GAME_CHUNK_RADIUS = 4;
export const int GAME_CHUNK_MAX = (GAME_CHUNK_RADIUS * GAME_CHUNK_RADIUS * 4) + (GAME_CHUNK_RADIUS * 10);

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
    
public:
    void buildMap();

    void start() override;
    void update() override;

    GameScene() = default;
    virtual ~GameScene() = default;
};

void GameScene::buildMap()
{
    auto texture = ServiceProvider::getAssetManager()->getTexture("blocks.png");

    std::shared_ptr<annileen::Shader> shader{ nullptr };
    if (ServiceProvider::getSettings()->getData()->shadows.enabled)
    {
        shader = ServiceProvider::getAssetManager()->getShader("lit_shadow");
    }
    else
    {
        shader = ServiceProvider::getAssetManager()->getShader("lit_noshadow");
    }

    auto shaderPass = std::make_shared<ShaderPass>();
    shaderPass->init(shader);
    shaderPass->setState(BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | UINT64_C(0));

    m_BlockMaterial = std::make_shared<Material>();
    m_BlockMaterial->addShaderPass(shaderPass);
    m_BlockMaterial->setName("BlockMaterial");

    m_BlockMaterial->setTexture("s_mainTex", texture);

    fog.color = glm::vec3(0.823f, 0.705f, 0.513f);
    fog.distance = 150.0f;
    fog.enabled = 1.0f;
    fog.power = 1.3f;

    auto lightNode = createNode("Light");
    auto light = SceneManager::getInstance()->addModule<Light>(this, lightNode);

    light->color = glm::vec3(1.0f, 1.0f, .8f);
    light->type = LightType::Directional;
    light->intensity = 0.8f;
    light->getTransform().rotate(glm::vec3(-40.0f, 0.0f, -40.0f));

    auto cameraNode = createNode("Camera");

    auto camera = SceneManager::getInstance()->addModule<Camera>(this, cameraNode);
    camera->fieldOfView = 60.0f;
    camera->nearClip = 0.1f;
    camera->farClip = 300.0f;

    auto textNode = createNode("Text");
    auto text = SceneManager::getInstance()->addModule<Text>(this, textNode);
    text->setStatic(true);
    text->setSdf(true);

    float screenWidth = static_cast<float>(ServiceProvider::getSettings()->getData()->windowResolution.x);
    text->setFont(ServiceProvider::getAssetManager()->getFont("droidsans.ttf")->getHandle());
    text->setScreenPosition(screenWidth - 300.0f, 200.0f);
    text->setTextColor(glm::vec3(1, 0, 0));
    text->setBackgroundColor(glm::vec3(0.5));
    text->setStyle(Text::TextStyle::Background);
    text->setText("This is a Annileen\nUsing SDF");

    auto textNode2 = createNode("Text2");
    auto text2 = SceneManager::getInstance()->addModule<Text>(this, textNode2);

    text2->setFont(ServiceProvider::getAssetManager()->getFont("bleeding_cowboys.ttf")->getHandle());
    text2->setScreenPosition(screenWidth - 200.0f, 300.0f);
    text2->setTextColor(glm::vec3(0, 1, 0));
    text2->setStyle(Text::TextStyle::StrikeThrough);
    text2->setText("OH YEAH");

    auto cubemap = ServiceProvider::getAssetManager()->getCubemap("skybox.toml");
    auto skybox = std::make_shared<Skybox>(cubemap);
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
    auto cameraPos = getCamera()->getTransform().position();

    bool k = false;
    uint64_t ikill;
    float dist = 0.0f;

    for (const auto& c : m_AvailableChunks)
    {
        if (c.second == nullptr)
        {
            continue;
        }

        auto d = glm::abs(glm::length(cameraPos - c.second->getSceneNode(this)->getTransform().position()));
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
        delete chunk;
    }
}

void GameScene::addChunk(Chunk* chunk)
{
    // Get it, so it gets built
    chunk->getSceneNode(this);
}

void GameScene::start()
{
    buildMap();

    getCamera()->getTransform().position(glm::vec3(0.0f, 70.0f, 0.0f));
}

void GameScene::update()
{
    // know exactly what chunks to generate and render
    // TODO: the render/occlusion part will have to be moved over to a better place later
    //clearChunks();

    if (getCamera() == nullptr)
    {
        return;
    }

    auto cameraPos = getCamera()->getTransform().position();
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
