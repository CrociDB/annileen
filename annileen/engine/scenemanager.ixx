module;

#include <unordered_map>
#include <list>
#include <engine/forward_decl.h>
#include <type_traits>

export module scenemanager;

import scene;
import scenenode;
import light;
import camera;

export namespace annileen
{
    class SceneManager
    {
    private:
        static SceneManager* s_Instance;

        Scene* m_Scene{ nullptr };
        
        SceneManager() = default;
        ~SceneManager() = default;

    public: 
        static SceneManager* getInstance();

        template<typename T>
        T* createScene();

        //void setScene(Scene* scene);
        Scene* getScene() const;

        void setParentScene(Scene* scene, SceneNode* sceneNode);
        void destroySceneNode(Scene* scene, SceneNode* sceneNode);

        void addCameraModule(Scene* scene, Camera* camera);
        void addLightModule(Scene* scene, Light* light);
        void removeCameraModule(Scene* scene, Camera* camera);
        void removeLightModule(Scene* scene, Light* light);

        template <class T> T* getModule(SceneNode* sceneNode) const;
        template <class T> T* addModule(Scene* scene, SceneNode* sceneNode);
        template <class T> bool removeModule(Scene* scene, SceneNode* sceneNode);
    };
}

namespace annileen
{
    SceneManager* SceneManager::s_Instance{ nullptr };

    SceneManager* SceneManager::getInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new SceneManager();
        }

        return s_Instance;
    }

    template<typename T>
    T* SceneManager::createScene()
    {
        static_assert(std::is_base_of<Scene, T>::value, "T must derive from Scene");
        delete m_Scene;
        T* newScene = new T();
        m_Scene = dynamic_cast<Scene*>(newScene);
        return newScene;
    }

    template <class T>
    T* SceneManager::getModule(SceneNode* sceneNode) const
    {
        auto moduleIt = sceneNode->m_Modules.find(typeid(T));

        if (moduleIt != sceneNode->m_Modules.end())
        {
            return static_cast<T*>(moduleIt->second);
        }

        return nullptr;
    }

    template <class T>
    T* SceneManager::addModule(Scene* scene, SceneNode* sceneNode)
    {
        if (!std::is_base_of<SceneNodeModule, T>::value)
        {
            //ANNILEEN_LOGF_ERROR(LoggingChannel::General, "\"{0}\" cannot be added because it is not a module.", typeid(T).name());
            return nullptr;
        }

        auto moduleIt = sceneNode->m_Modules.find(typeid(T));

        if (moduleIt != sceneNode->m_Modules.end())
        {
            //ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode has a Module of this type already. Remove the existing module before adding a new one of the same type.");
            return nullptr;
        }

        T* sceneNodeModule = new T();

        sceneNode->m_Modules[typeid(T)] = static_cast<SceneNodeModulePtr>(sceneNodeModule);

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
    bool SceneManager::removeModule(Scene* scene, SceneNode* sceneNode)
    {
        auto moduleIt = sceneNode->m_Modules.find(typeid(T));

        if (moduleIt == sceneNode->m_Modules.end())
        {
            //ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode does not have a Module of this type.");
            return false;
        }

        T* sceneNodeModule = static_cast<T*>(moduleIt->second);

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

            delete sceneNodeModule;
            sceneNodeModule = nullptr;
        }

        sceneNode->m_Modules.erase(moduleIt);

        return true;
    }

    /*void SceneManager::setScene(Scene* scene)
    {
        m_Scene = scene;
    }*/

    Scene* SceneManager::getScene() const
    {
        return m_Scene;
    }

    void SceneManager::setParentScene(Scene* scene, SceneNode* sceneNode)
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

    void SceneManager::destroySceneNode(Scene* scene, SceneNode* sceneNode)
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
            SceneNodeModulePtr sceneNodeModule = moduleIt.second;

            if (sceneNodeModule != nullptr)
            {
                if (scene != nullptr)
                {
                    if (moduleIt.first == typeid(Camera))
                    {
                        Camera* camera = static_cast<Camera*>(sceneNodeModule);
                        scene->m_Cameras.remove(camera);
                        camera = nullptr;
                    }
                    else if (moduleIt.first == typeid(Light))
                    {
                        Light* light = static_cast<Light*>(sceneNodeModule);
                        scene->m_Lights.remove(light);
                        light = nullptr;
                    }
                }

                delete sceneNodeModule;
                sceneNodeModule = nullptr;
            }
        }

        sceneNode->deParent();
        sceneNode->m_Modules.clear();
        sceneNode->m_Parent = nullptr;

        delete sceneNode;
        sceneNode = nullptr;
    }

    void SceneManager::addCameraModule(Scene* scene, Camera* camera)
    {
        scene->m_Cameras.push_back(camera);
    }

    void SceneManager::addLightModule(Scene* scene, Light* light)
    {
        scene->m_Lights.push_back(light);
    }

    void SceneManager::removeCameraModule(Scene* scene, Camera* camera)
    {
        scene->m_Cameras.remove(camera);
    }

    void SceneManager::removeLightModule(Scene* scene, Light* light)
    {
        scene->m_Lights.remove(light);
    }
}