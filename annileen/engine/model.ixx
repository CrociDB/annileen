module;

#include <iostream>
#include <vector>

export module model;

import scenenode;
import mesh;
import material;

export namespace annileen
{
	class Model final : public SceneNodeModule
	{
	public:	
		Model();
		virtual ~Model();

		void init(std::shared_ptr<MeshGroup> meshGroup, std::shared_ptr<Material> material);
		void setMeshGroup(std::shared_ptr<MeshGroup> meshGroup);
		std::shared_ptr<MeshGroup> getMeshGroup() const;
		std::shared_ptr<Material> getMaterial() const;

	public:
		bool castShadows{ true };
		bool receiveShadows{ true };

		// Should this be moved to a parent component class? Will model be a "component"?
		bool isStatic{ false };
		bool enabled{ true };

	private:
		std::shared_ptr<MeshGroup> m_MeshGroup{ nullptr };
		std::shared_ptr<Material> m_Material{ nullptr };
	};
}

module :private;

namespace annileen
{
	Model::~Model()
	{
		// TODO: remove
		std::cout << "Model destroyed." << std::endl;
	}

	void Model::init(std::shared_ptr<MeshGroup> meshGroup, std::shared_ptr<Material> material)
	{
		m_MeshGroup = meshGroup;
		m_Material = material;
	}

	void Model::setMeshGroup(std::shared_ptr<MeshGroup> meshGroup) 
	{ 
		m_MeshGroup = meshGroup; 
	}

	std::shared_ptr<MeshGroup> Model::getMeshGroup() const 
	{ 
		return m_MeshGroup; 
	}

	std::shared_ptr<Material> Model::getMaterial() const
	{
		return m_Material;
	}

	Model::Model() : SceneNodeModule() {}
}
