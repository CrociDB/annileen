#include <engine/model.h>

namespace annileen
{
	void Model::init(MeshGroup* meshGroup, std::shared_ptr<Material> material)
	{
		m_MeshGroup = meshGroup;
		m_Material = material;
	}

	MeshGroup* Model::getMeshGroup()
	{
		return m_MeshGroup;
	}

	std::shared_ptr<Material> Model::getMaterial()
	{
		return m_Material;
	}

	Model::Model() : SceneNodeModule(), m_MeshGroup(nullptr), m_Material(nullptr), castShadows(true),
		receiveShadows(true), isStatic(false), enabled(true)
	{
	}

	Model::~Model()
	{

	}
}
