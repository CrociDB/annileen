#ifndef _RAWMESH_H_
#define _RAWMESH_H_

#include <iostream>
#include <vector>

#include <glm.hpp>

#include "mesh.h"

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
public:
	std::vector<RawVertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	Mesh* getMesh();
};

#endif
