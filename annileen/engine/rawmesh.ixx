module; 

#include <iostream>
#include <vector>
#include <glm.hpp>
#include <bgfx/bgfx.h>

export module anni.rawmesh;

import anni.mesh;
import anni.asset;
import anni.allocators;

export namespace annileen
{
	struct RawVertex
	{
		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec3 m_Tangent;
		glm::vec2 m_UV;

		bool m_HasNormal;
		bool m_HasUV;
	};

	class RawMesh
	{
	public:		
		void getMesh(std::shared_ptr<Mesh> mesh, const MeshDescriptor& descriptor);
	private:
		void generateNormals();
	
	public:
		std::vector<RawVertex> m_Vertices{};
		std::vector<uint32_t> m_Indices{};
	};
}

module :private;

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
			glm::vec3 t = glm::normalize(glm::cross(glm::normalize(p2 - p1), n));

			m_Vertices[m_Indices[i]].m_Normal = n;
			m_Vertices[m_Indices[i + 1]].m_Normal = n;
			m_Vertices[m_Indices[i + 2]].m_Normal = n;

			m_Vertices[m_Indices[i]].m_Tangent = t;
			m_Vertices[m_Indices[i + 1]].m_Tangent = t;
			m_Vertices[m_Indices[i + 2]].m_Tangent = t;

			m_Vertices[m_Indices[i]].m_HasNormal = true;
			m_Vertices[m_Indices[i + 1]].m_HasNormal = true;
			m_Vertices[m_Indices[i + 2]].m_HasNormal = true;
		}
	}

	void RawMesh::getMesh(std::shared_ptr<Mesh> mesh, const MeshDescriptor& descriptor)
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
		if (vertexNormal)
		{
			vlayout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);
			vlayout.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float);
		}

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

				vertexData.push_back((float)v.m_Tangent.x);
				vertexData.push_back((float)v.m_Tangent.y);
				vertexData.push_back((float)v.m_Tangent.z);
			}

		}

		float* meshData = new float[vertexData.size()];
		memcpy(meshData, vertexData.data(), vertexData.size() * sizeof(float));

		uint32_t* indexData = new uint32_t[m_Indices.size()];
		memcpy(indexData, m_Indices.data(), m_Indices.size() * sizeof(uint32_t));

		mesh->init(
			bgfx::makeRef(meshData, static_cast<uint32_t>(vertexData.size() * sizeof(float)), Allocators::releaseMem), 
			vlayout,
			bgfx::makeRef(indexData, static_cast<uint32_t>(m_Indices.size() * sizeof(uint32_t)), Allocators::releaseMem));
 	}
}
