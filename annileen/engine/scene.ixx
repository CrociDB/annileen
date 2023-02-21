module;

#include <iostream>
#include <list>
#include <memory>
#include <glm.hpp>

export module scene;

import scenenode;
import camera;
import light;
import skybox;

export namespace annileen
{
    struct Fog
    {
        glm::vec3 color;
        float distance;
        float power;
        bool enabled;
    };

    class Scene
    {
    public:
        friend class SceneManager;
        friend class SceneNode;

        Scene();
        virtual ~Scene();

        virtual void start() {};
        virtual void update() {};

        std::shared_ptr<SceneNode> createNode(const std::string& name);       

        void setSkybox(std::shared_ptr<Skybox> skybox) noexcept;
        std::shared_ptr<Skybox> getSkybox() const noexcept;        
        std::shared_ptr<SceneNode> getRoot() const noexcept;
        std::list<std::shared_ptr<SceneNode>>& getNodeList() noexcept;
        std::list<Light*>& getLightList() noexcept;
        std::list<Camera*>& getCameraList() noexcept;
        Camera* getCamera() const noexcept;

    private:
        void addNodeToList(std::shared_ptr<SceneNode> node);
        void removeNodeFromList(std::shared_ptr<SceneNode> node);

    public:
        Fog fog{};

    private:
        std::list<std::shared_ptr<SceneNode>> m_Nodes;
        std::list<Light*> m_Lights;
        std::list<Camera*> m_Cameras;
        std::shared_ptr<SceneNode> m_Root{ nullptr };
        std::shared_ptr<Skybox> m_Skybox{ nullptr };
    };
}

namespace annileen
{
    void Scene::setSkybox(std::shared_ptr<Skybox> skybox) noexcept 
    { 
        m_Skybox = skybox; 
    }

    std::shared_ptr<Skybox> Scene::getSkybox() const noexcept 
    { 
        return m_Skybox; 
    }

    std::shared_ptr<SceneNode> Scene::getRoot() const noexcept
    {
        return m_Root;
    }

    std::shared_ptr<SceneNode> Scene::createNode(const std::string& name)
    {
        auto node{ std::make_shared<SceneNode>(name) };
        m_Nodes.push_back(node);
        node->setParent(getRoot());
        return node;
    }

    void Scene::addNodeToList(std::shared_ptr<SceneNode> node)
    {
        m_Nodes.push_back(node);
    }

    void Scene::removeNodeFromList(std::shared_ptr<SceneNode> node)
    {
        m_Nodes.remove(node);
    }

    std::list<std::shared_ptr<SceneNode>>& Scene::getNodeList() noexcept
    {
        return m_Nodes;
    }

    std::list<Light*>& Scene::getLightList() noexcept
    {
        return m_Lights;
    }

    std::list<Camera*>& Scene::getCameraList() noexcept
    {
        return m_Cameras;
    }

    Camera* Scene::getCamera() const noexcept
    {
        for (auto camera : m_Cameras)
        {
            if (camera->getSceneNode()->getActive() && camera->enabled)
            {
                return camera;
            }
        }
        return nullptr;
    }

    Scene::Scene()
    {
        //m_Camera = new Camera(60.0f, 0.1f, 300.0f);
        m_Root = std::make_shared<SceneNode>("Root");
    }

    Scene::~Scene()
    {
        m_Nodes.clear();
        m_Cameras.clear();
        m_Lights.clear();

        // TODO: remove
        std::cout << "Scene destroyed." << std::endl;
    }
}
