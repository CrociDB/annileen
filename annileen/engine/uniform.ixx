module;

#include <gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <bgfx/bgfx.h>
#include <glm.hpp>

export module anni.uniform;

import anni.texture;
import anni.cubemap;

export namespace annileen
{
	struct UniformData
	{
		bgfx::UniformHandle m_Handle;
		bgfx::UniformType::Enum m_Type;
	};

	class Uniform final
	{
	private:
		static std::map<std::string, UniformData> m_Uniforms;

		static bgfx::UniformHandle createUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype);
		static bgfx::UniformHandle getOrCreateUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype);

	public:
		static void resetUniformList();

		static bgfx::UniformHandle getSamplerUniformHandle(const std::string& uniformname);
		static bgfx::UniformHandle getVec4UniformHandle(const std::string& uniformname);
		static bgfx::UniformHandle getSMat3UniformHandle(const std::string& uniformname);
		static bgfx::UniformHandle getMat4UniformHandle(const std::string& uniformname);

		static void setCubemapUniform(const std::string& uniformname, std::shared_ptr<const Cubemap> value, uint8_t registerId);
		static void setTextureUniform(const std::string& uniformname, std::shared_ptr<const Texture> value, uint8_t registerId);
		static void setColorUniform(const std::string& uniformname, const glm::vec4& value);
		static void setVec4Uniform(const std::string& uniformname, const glm::vec4& value);
		static void setVec3Uniform(const std::string& uniformname, const glm::vec3& value);
		static void setVec2Uniform(const std::string& uniformname, const glm::vec2& value);
		static void setFloatUniform(const std::string& uniformname, const float& value);
		static void setMat4Uniform(const std::string& uniformname, const glm::mat4& value);

		static void destroy();

		Uniform() = delete;
		~Uniform() = delete;
	};
}

module :private;

namespace annileen
{
	std::map<std::string, UniformData> Uniform::m_Uniforms{};

	bgfx::UniformHandle Uniform::createUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype)
	{
		UniformData u{ bgfx::createUniform(uniformname.c_str(),  uniformtype), uniformtype };
		m_Uniforms[uniformname] = u;
		return u.m_Handle;
	}

	bgfx::UniformHandle Uniform::getOrCreateUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype)
	{
		try
		{
			auto u = m_Uniforms.at(uniformname);
			return u.m_Handle;
		}
		catch (std::out_of_range&) {}

		return createUniform(uniformname, uniformtype);
	}


	void Uniform::resetUniformList()
	{
		m_Uniforms.clear();
	}

	bgfx::UniformHandle Uniform::getSamplerUniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Sampler);
	}

	bgfx::UniformHandle Uniform::getVec4UniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Vec4);
	}

	bgfx::UniformHandle Uniform::getSMat3UniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Mat3);
	}

	bgfx::UniformHandle Uniform::getMat4UniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Mat4);
	}

	void Uniform::setCubemapUniform(const std::string& uniformname, std::shared_ptr<const Cubemap> value, uint8_t registerId)
	{
		bgfx::setTexture(registerId, getSamplerUniformHandle(uniformname), value->getHandle());
	}

	void Uniform::setTextureUniform(const std::string& uniformname, std::shared_ptr<const Texture> value, uint8_t registerId)
	{
		bgfx::setTexture(registerId, getSamplerUniformHandle(uniformname), value->getHandle());
	}

	void Uniform::setColorUniform(const std::string& uniformname, const glm::vec4& value)
	{
		setVec4Uniform(uniformname, value);
	}

	void Uniform::setVec4Uniform(const std::string& uniformname, const glm::vec4& value)
	{ 
		bgfx::setUniform(getVec4UniformHandle(uniformname), glm::value_ptr(value), 1);
	}

	void Uniform::setVec3Uniform(const std::string& uniformname, const glm::vec3& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), glm::value_ptr(value), 1);
	}

	void Uniform::setVec2Uniform(const std::string& uniformname, const glm::vec2& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), glm::value_ptr(value), 1);
	}

	void Uniform::setFloatUniform(const std::string& uniformname, const float& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), &value, 1);
	}

	void Uniform::setMat4Uniform(const std::string& uniformname, const glm::mat4& value)
	{
		bgfx::setUniform(getMat4UniformHandle(uniformname), glm::value_ptr(value), 1);
	}

	void Uniform::destroy()
	{
		for (const auto& [k, v] : m_Uniforms)
		{
			bgfx::destroy(v.m_Handle);
		}

		m_Uniforms.clear();
	}
}
