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

        void setSkybox(Skybox* skybox) { m_Skybox = skybox; }
        Skybox* getSkybox() const { return m_Skybox; }

        void clearNodes();
        std::list<SceneNodePtr>& getNodeList();
        std::list<Light*>& getLightList();
        std::list<Camera*>& getCameraList();
        Camera* getCamera();        

        Scene();
        ~Scene();
    };
}

#endif
