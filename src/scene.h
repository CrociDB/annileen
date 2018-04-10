#ifndef _SCENE_H_
#define _SCENE_H_

#include <iostream>
#include <list>
#include <memory>

#include "mesh.h"

class Scene
{
private:
    std::list<std::shared_ptr<Mesh>> m_Meshes;

public:
    void addMesh(Mesh* mesh);

    std::list<std::shared_ptr<Mesh>>& getMeshList();

    Scene();
    ~Scene();
};

#endif
