module;

#include <iostream>
#include <vector>
#include <engine/forward_decl.h>

export module model;

import scenenodemodule;
import mesh;
import material;

export namespace annileen
{

	class Model final : public SceneNodeModule
	{
	private:
		MeshGroup* m_MeshGroup = nullptr;
		std::shared_ptr<Material> m_Material = nullptr;

	public:
		bool castShadows;
		bool receiveShadows;

		// Should this be moved to a parent component class? Will model be a "component"?
		bool isStatic;
		bool enabled;

		void init(MeshGroup* meshGroup, std::shared_ptr<Material> material);

		void setMeshGroup(MeshGroup* meshGroup) { m_MeshGroup = meshGroup; }
		MeshGroup* getMeshGroup() const { return m_MeshGroup; }
		std::shared_ptr<Material> getMaterial();

		Model();
		~Model();
	};

}


namespace annileen
{
	void Model::init(MeshGroup* meshGroup, std::shared_ptr<Material> material)
	{
		m_MeshGroup = meshGroup;
		m_Material = material;
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
