#include "model.h"

namespace annileen
{
	void Model::init(Mesh* mesh, std::shared_ptr<Material> material)
	{
		m_Mesh = mesh;
		m_Material = material;

		m_State = BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			| UINT64_C(0);
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
