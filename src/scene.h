#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "mesh.h"
#include "camera.h"
#include "cubemap.h"
#include "skybox.h"

class Scene
{
private:
    std::list<std::shared_ptr<Mesh>> m_Meshes;
    Camera* m_Camera;
    Cubemap* m_Cubemap;
    Skybox* m_Skybox;

    void createSkybox();

public:
    void addMesh(Mesh* mesh);
    void setCubemap(Cubemap* cubemap);

    Cubemap* getCubemap() const;
    Skybox* getSkybox() const;

    std::list<std::shared_ptr<Mesh>>& getMeshList();
    Camera* getCamera();

    Scene();
    ~Scene();
};

#endif
