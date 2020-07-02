#include "rawmesh.h"

void RawMesh::generateNormals()
{
	for (uint32_t i = 0; i < m_Indices.size() - 2; i+=3)
	{
		glm::vec3 p1 = m_Vertices[m_Indices[i]].m_Position;
		glm::vec3 p2 = m_Vertices[m_Indices[i+1]].m_Position;
		glm::vec3 p3 = m_Vertices[m_Indices[i+2]].m_Position;

		glm::vec3 n = glm::normalize(glm::cross(glm::normalize(p2 - p1), glm::normalize(p3 - p1)));

		m_Vertices[m_Indices[i]].m_Normal = n;
		m_Vertices[m_Indices[i+1]].m_Normal = n;
		m_Vertices[m_Indices[i+2]].m_Normal = n;

		m_Vertices[m_Indices[i]].m_HasNormal = true;
		m_Vertices[m_Indices[i+1]].m_HasNormal = true;
		m_Vertices[m_Indices[i+2]].m_HasNormal = true;
	}
}

Mesh* RawMesh::getMesh()
{
	generateNormals();

	Mesh* mesh = new Mesh();

	bool vertexColor = false;
	bool vertexNormal = m_Vertices[0].m_HasNormal;
	bool vertexUV = m_Vertices[0].m_HasUV;

	uint8_t vertexAttr = (vertexColor) | (vertexNormal << 2) | (vertexUV << 1);

	std::vector<float> vertexData;

	for (const auto& v : m_Vertices)
	{
		vertexData.push_back(v.m_Position.x);
		vertexData.push_back(v.m_Position.y);
		vertexData.push_back(v.m_Position.z);

		if (vertexNormal)
		{
			vertexData.push_back(v.m_Normal.x);
			vertexData.push_back(v.m_Normal.y);
			vertexData.push_back(v.m_Normal.z);
		}

		if (vertexUV)
		{
			vertexData.push_back(v.m_UV.x);
			vertexData.push_back(v.m_UV.y);
		}
	}

	mesh->init(vertexData.data(), vertexData.size(), vertexAttr, m_Indices.data(), m_Indices.size());

	return mesh;
}
