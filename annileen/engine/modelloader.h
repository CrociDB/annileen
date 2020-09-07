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
        void processNode(aiNode* node, const aiScene* scene, MeshGroup* meshGroup, const MeshDescriptor& descriptor);
        void convertMesh(Mesh* mesh, aiMesh* aiMesh, const aiScene* scene, const MeshDescriptor& descriptor);
    public:
        ModelLoader();
        ~ModelLoader();

        MeshGroup* loadMesh(const std::string& filename, const MeshDescriptor& descriptor);
    };
}
