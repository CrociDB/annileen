#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "mesh.h"
#include "camera.h"

class Scene
{
private:
    std::list<std::shared_ptr<Mesh>> m_Meshes;
    Camera* m_Camera;

public:
    void addMesh(Mesh* mesh);

    std::list<std::shared_ptr<Mesh>>& getMeshList();
    Camera* getCamera();

    Scene();
    ~Scene();
};

#endif
