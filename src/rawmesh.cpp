#include "rawmesh.h"

Mesh* RawMesh::getMesh()
{
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
