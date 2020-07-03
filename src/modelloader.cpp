#include "modelloader.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

std::vector<Mesh*> ModelLoader::loadMesh(const std::string& filename)
{
	std::vector<Mesh*> meshes;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "[Error] Assimp " << importer.GetErrorString() << std::endl;
		exit(-1);
	}

	std::string directory = filename.substr(0, filename.find_last_of('/'));
	
	processNode(scene->mRootNode, scene, meshes);

	return meshes;
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes)
{
	for (uint16_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		auto converted_mesh = convertMesh(mesh, scene);
		meshes.push_back(converted_mesh);
	}

	for (uint16_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, meshes);
	}
}

Mesh* ModelLoader::convertMesh(aiMesh* mesh, const aiScene* scene)
{
	RawMesh rawMesh;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		RawVertex v;

		v.m_Position.x = mesh->mVertices[i].x;
		v.m_Position.y = mesh->mVertices[i].y;
		v.m_Position.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			v.m_UV.x = mesh->mTextureCoords[0][i].x;
			v.m_UV.y = mesh->mTextureCoords[0][i].y;
			v.m_HasUV = true;
		}
		else
		{
			v.m_HasUV = false;
		}

		if (mesh->mNormals)
		{
			v.m_Normal.x = mesh->mNormals[i].x;
			v.m_Normal.y = mesh->mNormals[i].y;
			v.m_Normal.z = mesh->mNormals[i].z;
			v.m_HasNormal = true;
		}
		else
		{
			v.m_HasNormal = false;
		}

		rawMesh.m_Vertices.push_back(v);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
			rawMesh.m_Indices.push_back(face.mIndices[j]);
	}

	// Ignoring material loading for now
	return rawMesh.getMesh();
}
