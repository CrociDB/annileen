#pragma once

#include <iostream>
#include <vector>

#include <glm.hpp>

#include "mesh.h"

namespace annileen
{
	struct RawVertex
	{
		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec2 m_UV;

		bool m_HasNormal;
		bool m_HasUV;
	};

	class RawMesh
	{
	private:
		void generateNormals();

	public:
		std::vector<RawVertex> m_Vertices;
		std::vector<uint16_t> m_Indices;

		void getMesh(Mesh* mesh, const MeshDescriptor& descriptor);
	};
}
