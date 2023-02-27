module;

#include <bgfx/defines.h>
#include <bgfx/bgfx.h>

#include <memory>
#include <iostream>

export module skybox;

import cubemap;
import serviceprovider;
import shaderpass;
import material;
import mesh;
import shader;
import model;

const float s_skyboxCubeVertices[8][3] =
{
	{-1.0f,  1.0f,  1.0f},
	{ 1.0f,  1.0f,  1.0f},
	{-1.0f, -1.0f,  1.0f},
	{ 1.0f, -1.0f,  1.0f},
	{-1.0f,  1.0f, -1.0f},
	{ 1.0f,  1.0f, -1.0f},
	{-1.0f, -1.0f, -1.0f},
	{ 1.0f, -1.0f, -1.0f},
};

const uint32_t s_skyboxCubeTriList[] =
{
	0, 1, 2,
	1, 3, 2,
	4, 6, 5,
	5, 6, 7,
	0, 2, 4,
	4, 2, 6,
	1, 5, 3,
	5, 7, 3,
	0, 4, 1,
	4, 5, 1,
	2, 3, 6,
	6, 3, 7,
};

export namespace annileen
{
	class Skybox
	{
	public:
		Skybox(std::shared_ptr<Cubemap> cubemap);
		~Skybox();

		std::shared_ptr<Model> getModel() const;
		std::shared_ptr<Cubemap> getCubemap() const;
	
	private:
		void createModel();
	
	private:
		std::shared_ptr<Model> m_Model{ nullptr };
		std::shared_ptr<MeshGroup> m_MeshGroup{ nullptr };
		std::shared_ptr<Cubemap> m_Cubemap{ nullptr };
	};
}

module :private;

namespace annileen
{
	Skybox::~Skybox()
	{
		// TODO: remove
		std::cout << "Skybox destroyed." << std::endl;
	}

	std::shared_ptr<Model> Skybox::getModel() const 
	{ 
		return m_Model; 
	}

	std::shared_ptr<Cubemap> Skybox::getCubemap() const 
	{ 
		return m_Cubemap; 
	}

	void Skybox::createModel()
	{
		auto shader{ ServiceProvider::getAssetManager()->getShader("skybox") };
		auto material{ std::make_shared<Material>() };
		
		auto shaderPass{ std::make_shared<ShaderPass>() };
		shaderPass->init(shader);
		shaderPass->setState(BGFX_STATE_WRITE_RGB
			| BGFX_STATE_DEPTH_TEST_LEQUAL
			| BGFX_STATE_FRONT_CCW
			| BGFX_STATE_MSAA
			| 0);
		
		material->addShaderPass(shaderPass);
		material->setCubemap("s_texCube", m_Cubemap);

		bgfx::VertexLayout vlayout;
		vlayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();

		auto vdata{ bgfx::makeRef(s_skyboxCubeVertices, sizeof(s_skyboxCubeVertices)) };
		auto idata{ bgfx::makeRef(s_skyboxCubeTriList, sizeof(s_skyboxCubeTriList)) };

		m_MeshGroup = std::make_shared<MeshGroup>();
		auto mesh{ std::make_shared<Mesh>() };
		mesh->init(vdata, vlayout, idata);
		m_MeshGroup->m_Meshes.push_back(mesh);

		m_Model = std::make_shared<Model>();
		m_Model->init(m_MeshGroup, material);
	}

	Skybox::Skybox(std::shared_ptr<Cubemap> cubemap) : m_Cubemap(cubemap)
	{
		createModel();
	}	
}
