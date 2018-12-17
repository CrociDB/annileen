#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "mesh.h"
#include "camera.h"
#include "cubemap.h"
#include "skybox.h"
#include "light.h"

struct Fog
{
    glm::vec3 color;
    float distance;
    float power;
    bool enabled;
};

class Scene
{
protected:
    std::list<Mesh*> m_Meshes;
    std::list<Light*> m_Lights;
    Camera* m_Camera;
    Cubemap* m_Cubemap;
    Skybox* m_Skybox;

    void createSkybox();
public:
    Fog fog;

    void addMesh(Mesh* mesh);
    void removeMesh(Mesh* mesh);
    void clearMeshList();

    void addLight(Light* light);

    void setCubemap(Cubemap* cubemap);

    Cubemap* getCubemap() const;
    Skybox* getSkybox() const;

    std::list<Mesh*>& getMeshList();
    std::list<Light*>& getLightList();
    Camera* getCamera();

    Scene();
    ~Scene();
};

#endif
