#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "rawmesh.h"
#include "mesh.h"
#include "data.h"

class ModelLoader
{
private:
    void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes);
    Mesh* convertMesh(aiMesh* mesh, const aiScene* scene);
public:
    ModelLoader();
    ~ModelLoader();

    std::vector<Mesh*> loadMesh(const std::string& filename);
};

#endif