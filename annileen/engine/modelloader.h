#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "rawmesh.h"
#include "mesh.h"

namespace annileen
{
    class ModelLoader
    {
    private:
        void processNode(aiNode* node, const aiScene* scene, MeshGroup* meshGroup);
        void convertMesh(Mesh* mesh, aiMesh* aiMesh, const aiScene* scene);
    public:
        ModelLoader();
        ~ModelLoader();

        MeshGroup* loadMesh(const std::string& filename);
    };
}
