#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "mesh.h"
#include "camera.h"
#include "cubemap.h"

class Scene
{
private:
    std::list<std::shared_ptr<Mesh>> m_Meshes;
    Camera* m_Camera;
    Cubemap* m_Cubemap;

public:
    void addMesh(Mesh* mesh);
    void setCubemap(Cubemap* cubemap);

    Cubemap* getCubemap() const;

    std::list<std::shared_ptr<Mesh>>& getMeshList();
    Camera* getCamera();

    Scene();
    ~Scene();
};

#endif
