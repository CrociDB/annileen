#include "engine.h"
#include "rawmesh.h"

namespace annileen
{
	void RawMesh::generateNormals()
	{
		for (uint32_t i = 0; i < m_Indices.size() - 2; i += 3)
		{
			glm::vec3 p1 = m_Vertices[m_Indices[i]].m_Position;
			glm::vec3 p2 = m_Vertices[m_Indices[i + 1]].m_Position;
			glm::vec3 p3 = m_Vertices[m_Indices[i + 2]].m_Position;

			glm::vec3 n = glm::normalize(glm::cross(glm::normalize(p2 - p1), glm::normalize(p3 - p1)));

			m_Vertices[m_Indices[i]].m_Normal = n;
			m_Vertices[m_Indices[i + 1]].m_Normal = n;
			m_Vertices[m_Indices[i + 2]].m_Normal = n;

			m_Vertices[m_Indices[i]].m_HasNormal = true;
			m_Vertices[m_Indices[i + 1]].m_HasNormal = true;
			m_Vertices[m_Indices[i + 2]].m_HasNormal = true;
		}
	}

	void RawMesh::getMesh(Mesh* mesh, const MeshDescriptor& descriptor)
	{
		if (descriptor.m_Normals == MeshDescriptor::Normals::Generate)
			generateNormals();

		bool vertexColor = false;
		bool vertexNormal = m_Vertices[0].m_HasNormal;
		bool vertexUV = m_Vertices[0].m_HasUV;

		bgfx::VertexLayout vlayout;
		vlayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);

		if (vertexUV) vlayout.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
		if (vertexNormal) vlayout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);

		vlayout.end();

		std::vector<float> vertexData;

		for (const auto& v : m_Vertices)
		{
			vertexData.push_back(v.m_Position.x);
			vertexData.push_back(v.m_Position.y);
			vertexData.push_back(v.m_Position.z);

			if (vertexUV)
			{
				vertexData.push_back(v.m_UV.x);
				vertexData.push_back(v.m_UV.y);
			}

			if (vertexNormal)
			{
				vertexData.push_back((float)v.m_Normal.x);
				vertexData.push_back((float)v.m_Normal.y);
				vertexData.push_back((float)v.m_Normal.z);
			}

		}

		float* meshData = new float[vertexData.size()];
		memcpy(meshData, vertexData.data(), vertexData.size() * sizeof(float));

		uint16_t* indexData = new uint16_t[m_Indices.size()];
		memcpy(indexData, m_Indices.data(), m_Indices.size() * sizeof(uint16_t));

		mesh->init(
			bgfx::makeRef(meshData, vertexData.size() * sizeof(float), Engine::releaseMem), 
			vlayout,
			bgfx::makeRef(indexData, m_Indices.size() * sizeof(uint16_t), Engine::releaseMem));
 	}
}
