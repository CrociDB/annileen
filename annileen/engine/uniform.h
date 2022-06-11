#pragma once

#include <iostream>
#include <map>

#include <bgfx/bgfx.h>
#include <glm.hpp>

#include "texture.h"
#include "cubemap.h"

namespace annileen
{
	struct UniformData
	{
		bgfx::UniformHandle m_Handle;
		bgfx::UniformType::Enum m_Type;
	};

	class Uniform
	{
	private:
		std::map<std::string, UniformData> m_Uniforms;

		bgfx::UniformHandle createUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype);
		bgfx::UniformHandle getOrCreateUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype);

	public:
		void resetUniformList();

		bgfx::UniformHandle getSamplerUniformHandle(const std::string& uniformname);
		bgfx::UniformHandle getVec4UniformHandle(const std::string& uniformname);
		bgfx::UniformHandle getSMat3UniformHandle(const std::string& uniformname);
		bgfx::UniformHandle getMat4UniformHandle(const std::string& uniformname);

		void setCubemapUniform(const std::string& uniformname, const Cubemap* value, uint8_t registerId);
		void setTextureUniform(const std::string& uniformname, const Texture* value, uint8_t registerId);
		void setColorUniform(const std::string& uniformname, const glm::vec4& value);
		void setVec4Uniform(const std::string& uniformname, const glm::vec4& value);
		void setVec3Uniform(const std::string& uniformname, const glm::vec3& value);
		void setVec2Uniform(const std::string& uniformname, const glm::vec2& value);
		void setFloatUniform(const std::string& uniformname, const float& value);
		void setMat4Uniform(const std::string& uniformname, const glm::mat4& value);

		void destroy();

		Uniform();
		~Uniform();
	};
}
