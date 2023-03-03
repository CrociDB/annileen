module;

#include <unordered_map>
#include <list>
#include <type_traits>
#include <memory>
#include <iostream>

export module anni.scenemanager;

import anni.scene;
import anni.scenenode;
import anni.light;
import anni.camera;

export namespace annileen
{
    class SceneManager
    {
    private:
        friend class Engine;

        static SceneManager* s_Instance;

        SceneManager() = default;
        ~SceneManager();

        static void destroy();

    public: 
        static SceneManager* getInstance();

        //void setScene(Scene* scene);
        std::shared_ptr<Scene> getScene() const noexcept;

        void setParentScene(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode);
        void destroySceneNode(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode);

        void addCameraModule(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        void addLightModule(std::shared_ptr<Scene> scene, std::shared_ptr<Light> light);
        void removeCameraModule(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
        void removeLightModule(std::shared_ptr<Scene> scene, std::shared_ptr<Light> light);

        template <class T> std::shared_ptr<T> createScene();
        template <class T> std::shared_ptr<T> getModule(std::shared_ptr<SceneNode> sceneNode) const;
        template <class T> std::shared_ptr<T> addModule(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode);
        template <class T> bool removeModule(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode);    
    
    private:
        std::shared_ptr<Scene> m_Scene{ nullptr };
    };
}

// Template implementations
namespace annileen
{
    template<class T>
    std::shared_ptr<T> SceneManager::createScene()
    {
        static_assert(std::is_base_of<Scene, T>::value, "T must derive from Scene");
        auto newScene{ std::make_shared<T>() };
        m_Scene = dynamic_pointer_cast<Scene>(newScene);
        return newScene;
    }

    template <class T>
    std::shared_ptr<T> SceneManager::getModule(std::shared_ptr<SceneNode> sceneNode) const
    {
        auto moduleIt{ sceneNode->m_Modules.find(typeid(T)) };

        if (moduleIt != sceneNode->m_Modules.end())
        {
            return static_pointer_cast<T>(moduleIt->second);
        }

        return nullptr;
    }

    template <class T>
    std::shared_ptr<T> SceneManager::addModule(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode)
    {
        if (!std::is_base_of<SceneNodeModule, T>::value)
        {
            //ANNILEEN_LOGF_ERROR(LoggingChannel::General, "\"{0}\" cannot be added because it is not a module.", typeid(T).name());
            return nullptr;
        }

        auto moduleIt{ sceneNode->m_Modules.find(typeid(T)) };

        if (moduleIt != sceneNode->m_Modules.end())
        {
            //ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode has a Module of this type already. Remove the existing module before adding a new one of the same type.");
            return nullptr;
        }

        auto sceneNodeModule{ std::make_shared<T>() };

        sceneNode->m_Modules[typeid(T)] = std::static_pointer_cast<SceneNodeModule>(sceneNodeModule);

        sceneNodeModule->m_SceneNode = sceneNode;

        if (!sceneNode->m_Internal)
        {
            if constexpr (std::is_same<T, Camera>::value)
            {
                addCameraModule(scene, sceneNodeModule);
            }
            else if constexpr (std::is_same<T, Light>::value)
            {
                addLightModule(scene, sceneNodeModule);
            }
        }

        return sceneNodeModule;
    }

    template <class T>
    bool SceneManager::removeModule(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode)
    {
        auto moduleIt{ sceneNode->m_Modules.find(typeid(T)) };

        if (moduleIt == sceneNode->m_Modules.end())
        {
            //ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode does not have a Module of this type.");
            return false;
        }

        auto sceneNodeModule{ std::static_pointer_cast<T>(moduleIt->second) };

        if (sceneNodeModule != nullptr)
        {
            sceneNodeModule->m_SceneNode = nullptr;

            if (!sceneNode->m_Internal)
            {
                if constexpr (std::is_same<T, Camera>::value)
                {
                    removeCameraModule(scene, sceneNodeModule);
                }
                else if constexpr (std::is_same<T, Light>::value)
                {
                    removeLightModule(scene, sceneNodeModule);
                }
            }
        }

        sceneNode->m_Modules.erase(moduleIt);

        return true;
    }
}

module :private;

namespace annileen
{
    SceneManager* SceneManager::s_Instance{ nullptr };

    SceneManager::~SceneManager()
    {
        auto sceneNodes = m_Scene->getNodeList();
        for (auto& sceneNode : sceneNodes)
        {
            destroySceneNode(m_Scene, sceneNode);
        }

        // TODO: remove
        std::cout << "SceneManager destroyed" << std::endl;
    }

    void SceneManager::destroy()
    {
        delete s_Instance;
    }

    SceneManager* SceneManager::getInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new SceneManager();
        }

        return s_Instance;
    }

    

    /*void SceneManager::setScene(Scene* scene)
    {
        m_Scene = scene;
    }*/

    std::shared_ptr<Scene> SceneManager::getScene() const noexcept
    {
        return m_Scene;
    }

    void SceneManager::setParentScene(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode)
    {
        //TODO: doesnt make sense because we need to add support to multiple scenes.
        if (scene != nullptr)
        {
            scene->removeNodeFromList(sceneNode);
        }

        if (scene != nullptr)
        {
            //m_ParentScene = scene;
            scene->addNodeToList(sceneNode);
        }
    }

    void SceneManager::destroySceneNode(std::shared_ptr<Scene> scene, std::shared_ptr<SceneNode> sceneNode)
    {
        if (scene != nullptr)
        {
            scene->removeNodeFromList(sceneNode);
        }

        for (auto child : sceneNode->m_Children)
        {
            if (child != nullptr)
            {
                destroySceneNode(scene, child);
            }
        }

        sceneNode->m_Children.clear();

        for (auto moduleIt : sceneNode->m_Modules)
        {
            auto sceneNodeModule{ moduleIt.second };

            if (sceneNodeModule != nullptr)
            {
                if (scene != nullptr)
                {
                    if (moduleIt.first == typeid(Camera))
                    {
                        auto camera{ static_pointer_cast<Camera>(sceneNodeModule) };
                        scene->m_Cameras.remove(camera);
                    }
                    else if (moduleIt.first == typeid(Light))
                    {
                        auto light{ static_pointer_cast<Light>(sceneNodeModule) };
                        scene->m_Lights.remove(light);
                    }
                }
            }
        }

        sceneNode->deParent();
        sceneNode->m_Modules.clear();
        sceneNode->m_Parent = nullptr;
    }

    void SceneManager::addCameraModule(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
    {
        scene->m_Cameras.push_back(camera);
    }

    void SceneManager::addLightModule(std::shared_ptr<Scene> scene, std::shared_ptr<Light> light)
    {
        scene->m_Lights.push_back(light);
    }

    void SceneManager::removeCameraModule(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera)
    {
        scene->m_Cameras.remove(camera);
    }

    void SceneManager::removeLightModule(std::shared_ptr<Scene> scene, std::shared_ptr<Light> light)
    {
        scene->m_Lights.remove(light);
    }
}