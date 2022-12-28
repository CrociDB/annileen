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
    private:
        std::list<SceneNodePtr> m_Nodes;
        SceneNodePtr m_Root;
        std::list<Light*> m_Lights;
        std::list<Camera*> m_Cameras;
        Skybox* m_Skybox = nullptr;

        void addNodeToList(SceneNodePtr node);
        void removeNodeFromList(SceneNodePtr node);

        friend class SceneNode;
    public:
        Fog fog;

        virtual void start() {};
        virtual void update() {};

        SceneNodePtr getRoot();

        SceneNodePtr createNode(const std::string& name);
        void destroyNode(SceneNodePtr node);

        void setSkybox(Skybox* skybox) { m_Skybox = skybox; }
        Skybox* getSkybox() const { return m_Skybox; }

        std::list<SceneNodePtr>& getNodeList();
        std::list<Light*>& getLightList();
        std::list<Camera*>& getCameraList();
        Camera* getCamera();

        Scene();
        ~Scene();
    };
}

namespace annileen
{
    SceneNodePtr Scene::getRoot()
    {
        return m_Root;
    }

    SceneNodePtr Scene::createNode(const std::string& name)
    {
        auto node = new SceneNode(name);
        node->setParentScene(this);
        node->setParent(getRoot());
        return node;
    }

    void Scene::destroyNode(SceneNodePtr node)
    {
        delete node;
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

    Camera* Scene::getCamera()
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

    Scene::Scene() : fog()
    {
        //m_Camera = new Camera(60.0f, 0.1f, 300.0f);
        m_Root = new SceneNode("Root");
    }

    Scene::~Scene()
    {
        if (m_Skybox != nullptr) delete m_Skybox;
        delete m_Root;
    }
}
