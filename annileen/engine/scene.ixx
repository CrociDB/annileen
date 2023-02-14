module;

#include <iostream>
#include <list>
#include <memory>
#include <glm.hpp>
#include <engine/forward_decl.h>

export module scene;

import scenenode;
import camera;
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
        friend class SceneManager;
        friend class SceneNode;

    public:
        Scene();
        virtual ~Scene();

        virtual void start() {};
        virtual void update() {};

        SceneNodePtr createNode(const std::string& name);       

        void setSkybox(Skybox* skybox) { m_Skybox = skybox; }
        Skybox* getSkybox() const { return m_Skybox; }
        
        SceneNodePtr getRoot() const;
        std::list<SceneNodePtr>& getNodeList();
        std::list<Light*>& getLightList();
        std::list<Camera*>& getCameraList();
        Camera* getCamera() const;

    private:
        void addNodeToList(SceneNodePtr node);
        void removeNodeFromList(SceneNodePtr node);

    public:
        Fog fog{};

    private:
        std::list<SceneNodePtr> m_Nodes;
        std::list<Light*> m_Lights;
        std::list<Camera*> m_Cameras;
        SceneNodePtr m_Root{ nullptr };
        Skybox* m_Skybox{ nullptr };
    };
}

namespace annileen
{
    SceneNodePtr Scene::getRoot() const
    {
        return m_Root;
    }

    SceneNodePtr Scene::createNode(const std::string& name)
    {
        auto node{ new SceneNode(name) };
        m_Nodes.push_back(node);
        node->setParent(getRoot());
        return node;
    }

    void Scene::addNodeToList(SceneNodePtr node)
    {
        m_Nodes.push_back(node);
    }

    void Scene::removeNodeFromList(SceneNodePtr node)
    {
        m_Nodes.remove(node);
    }

    std::list<SceneNodePtr>& Scene::getNodeList()
    {
        return m_Nodes;
    }

    std::list<Light*>& Scene::getLightList()
    {
        return m_Lights;
    }

    std::list<Camera*>& Scene::getCameraList()
    {
        return m_Cameras;
    }

    Camera* Scene::getCamera() const
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
        m_Root = new SceneNode("Root");
    }

    Scene::~Scene()
    {
        delete m_Skybox;
        
        for(auto sceneNode : m_Nodes)
        {
            delete sceneNode;
        }

        delete m_Root;

        // TODO: remove
        std::cout << "Scene destroyed." << std::endl;
    }
}
