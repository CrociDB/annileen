#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "scenenode.h"
//#include "rawmesh.h"
#include "camera.h"
//#include "cubemap.h"
//#include "skybox.h"
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
        std::list<SceneNode*> m_Nodes;
        SceneNode* m_Root;
        std::list<Light*> m_Lights;
        Camera* m_Camera;

    protected:
        /*Cubemap* m_Cubemap;
        Skybox* m_Skybox;*/

        void createSkybox();
    public:
        Fog fog;

        virtual void start() {};
        virtual void update() {};

        SceneNode* getRoot();
        SceneNode* createNode();

        /*void addMesh(Mesh* mesh);
        void removeMesh(Mesh* mesh);*/

        void addLight(Light* light);

        //void setCubemap(Cubemap* cubemap);

        /*Cubemap* getCubemap() const;
        Skybox* getSkybox() const;*/

        std::list<SceneNode*>& getNodeList();
        std::list<Light*>& getLightList();
        Camera* getCamera();

        Scene();
        ~Scene();
    };
}

#endif
