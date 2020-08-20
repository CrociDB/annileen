#include "engine.h"
#include "skybox.h"

namespace annileen
{
	void Skybox::createModel()
	{
		Shader* shader = Engine::getInstance()->getAssetManager()->loadShader("skybox.vs", "skybox.fs");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		
		std::shared_ptr<ShaderPass> shaderPass = std::make_shared<ShaderPass>();
		shaderPass->init(shader);
		shaderPass->setState(BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_LEQUAL
			| BGFX_STATE_FRONT_CCW
			| BGFX_STATE_MSAA
			| 0);
		
		material->addShaderPass(shaderPass);
		material->addCubemap("s_texCube", m_Cubemap, 0);

		bgfx::VertexLayout vlayout;
		vlayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();

		auto vdata = bgfx::makeRef(s_skyboxCubeVertices, sizeof(s_skyboxCubeVertices));
		auto idata = bgfx::makeRef(s_skyboxCubeTriList, sizeof(s_skyboxCubeTriList));

		Mesh* mesh = new Mesh();
		mesh->init(vdata, vlayout, idata, sizeof(s_skyboxCubeTriList) / sizeof(uint16_t));

		m_Model = std::make_shared<Model>();
		m_Model->init(mesh, material);
		
	}

	Skybox::Skybox(Cubemap* cubemap) : m_Cubemap(cubemap)
	{
		createModel();
	}
	
	Skybox::~Skybox()
	{
	}
}
