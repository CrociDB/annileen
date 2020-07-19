#pragma once

#include <iostream>
#include <vector>

#include "mesh.h"
#include "material.h"

namespace annileen
{
	class Model
	{
	private:
		Mesh* m_Mesh = nullptr;
		std::shared_ptr<Material> m_Material = nullptr;

		uint64_t m_State;
	public:
		void init(Mesh* mesh, std::shared_ptr<Material> material);

		void setState(uint64_t state) { m_State = state; }
		uint64_t getState() { return m_State; }

		Mesh* getMesh();
		std::shared_ptr<Material> getMaterial();

		Model();
		~Model();
	};
}
