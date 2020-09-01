#include <engine/model.h>

namespace annileen
{
	void Model::init(Mesh* mesh, std::shared_ptr<Material> material)
	{
		m_Mesh = mesh;
		m_Material = material;
	}

	Mesh* Model::getMesh()
	{
		return m_Mesh;
	}

	std::shared_ptr<Material> Model::getMaterial()
	{
		return m_Material;
	}

	Model::Model() : SceneNodeModule(), m_Mesh(nullptr), m_Material(nullptr), castShadows(true), 
		receiveShadows(true), isStatic(false), enabled(true)
	{
	}

	Model::~Model()
	{

	}
}
