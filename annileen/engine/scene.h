#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "scenenode.h"
#include "camera.h"
#include "skybox.h"
#include "light.h"

namespace annileen
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
        Camera* m_Camera;
        Skybox* m_Skybox = nullptr;

    public:
        Fog fog;

        virtual void start() {};
        virtual void update() {};

        SceneNodePtr getRoot();
        SceneNodePtr createNode();
        void removeNode(SceneNodePtr node);

        void addLight(Light* light);

        void setSkybox(Skybox* skybox) { m_Skybox = skybox; }
        Skybox* getSkybox() const { return m_Skybox; }

        void clearNodeList();
        std::list<SceneNodePtr>& getNodeList();
        std::list<Light*>& getLightList();
        Camera* getCamera();

        Scene();
        ~Scene();
    };
}

#endif
