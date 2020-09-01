#pragma once

#include <iostream>
#include <vector>

#include <engine/scenenodemodule.h>

namespace annileen
{
	class SceneNodeModule;
	class Mesh;
	class Material; 

	class Model final : public SceneNodeModule
	{
	private:
		Mesh* m_Mesh = nullptr;
		std::shared_ptr<Material> m_Material = nullptr;

	public:
		bool castShadows;
		bool receiveShadows;

		// Should this be moved to a parent component class? Will model be a "component"?
		bool isStatic;
		bool enabled;

		void init(Mesh* mesh, std::shared_ptr<Material> material);

		Mesh* getMesh();
		std::shared_ptr<Material> getMaterial();

		Model();
		~Model();
	};

	typedef Model* ModelPtr;
}
