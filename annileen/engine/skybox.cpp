#include <engine/engine.h>
#include <engine/skybox.h>
#include <engine/serviceprovider.h>
#include <engine/shaderpass.h>
#include <engine/material.h>
#include <engine/mesh.h>

namespace annileen
{
	void Skybox::createModel()
	{
		Shader* shader = ServiceProvider::getAssetManager()->loadShader("skybox.vs", "skybox.fs");
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

		m_MeshGroup = new MeshGroup();
		m_MeshGroup->m_Meshes.resize(1);
		m_MeshGroup->m_Meshes[0].init(vdata, vlayout, idata);

		m_Model = std::make_shared<Model>();
		m_Model->init(m_MeshGroup, material);
	}

	Skybox::Skybox(Cubemap* cubemap) : m_Cubemap(cubemap)
	{
		createModel();
	}
	
	Skybox::~Skybox()
	{
		delete m_MeshGroup;
	}
}
