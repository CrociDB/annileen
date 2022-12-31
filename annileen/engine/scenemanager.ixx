module;

#include <unordered_map>
#include <list>
#include <engine/forward_decl.h>

export module scenemanager;

import scene;
import scenenode;

export namespace annileen
{
    class SceneManager
    {
    private:
        Scene* m_Scene{ nullptr };
    public: 

        void setScene(Scene* scene);
        Scene* getScene() const;
    };
}

namespace annileen
{
    void SceneManager::setScene(Scene* scene)
    {
        m_Scene = scene;
    }

    Scene* SceneManager::getScene() const
    {
        return m_Scene;
    }
}