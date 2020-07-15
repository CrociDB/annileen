#include "model.h"

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

	Model::Model() : m_Mesh(nullptr)
	{

	}

	Model::~Model()
	{

	}
}
