#pragma once

#include <iostream>
#include <vector>

#include "scenenodemodule.h"
#include "mesh.h"

namespace annileen
{
	class SceneNodeModule;
	class Mesh;
	class Material; 

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

		MeshGroup* getMeshGroup();
		std::shared_ptr<Material> getMaterial();

		Model();
		~Model();
	};

	typedef Model* ModelPtr;
}
