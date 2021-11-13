#include <engine/material.h>
#include <engine/engine.h>
#include <algorithm>

//#include <engine/shaderpass.h>
//#include <engine/texture.h>
//#include <engine/cubemap.h>


namespace annileen
{
    void Material::addShaderPass(std::shared_ptr<ShaderPass> shaderPass)
    {
        m_ShaderPasses.push_back(shaderPass);

        auto shaderAvailableUniforms = shaderPass->getShaderAvailableUniforms();
        for (const auto& availableUniform : shaderAvailableUniforms)
        {
            addTexture(availableUniform.m_UniformName.c_str(), nullptr, availableUniform.m_Position, false);
        }
    }
    
    void Material::removeShaderPass(std::shared_ptr<ShaderPass> shaderPass)
    {
        auto shaderPassIt = std::find(m_ShaderPasses.begin(), m_ShaderPasses.end(), shaderPass);
        if (shaderPassIt != m_ShaderPasses.end())
        {
            m_ShaderPasses.erase(shaderPassIt);
        }
    }
    
    std::shared_ptr<ShaderPass> Material::getShaderPassAt(size_t shaderPassId)
    {
        if (shaderPassId >= 0 && shaderPassId < m_ShaderPasses.size())
        {
            return m_ShaderPasses.at(shaderPassId);
        }

        return nullptr;
    }

    size_t Material::getNumberOfShaderPasses() const
    {
        return m_ShaderPasses.size();
    }

    // Uniforms

    void Material::addTexture(const char* name, Texture* texture, uint8_t registerId, bool active)
    {
        MaterialSerializedUniform serializedUniform;
        serializedUniform.m_RegisterId = registerId;
        serializedUniform.m_Type = ShaderUniformType::Texture;
        serializedUniform.m_Texture = texture;
        serializedUniform.m_Active = active;
        m_SerializedUniforms[name] = serializedUniform;
    }

    void Material::addCubemap(const char* name, Cubemap* cubemap, uint8_t registerId, bool active)
    {
        MaterialSerializedUniform serializedUniform;
        serializedUniform.m_RegisterId = registerId;
        serializedUniform.m_Type = ShaderUniformType::Texture;
        serializedUniform.m_Cubemap = cubemap;
        serializedUniform.m_Active = active;
        m_SerializedUniforms[name] = serializedUniform;
    }

    void Material::setTexture(const char* name, Texture* texture)
    {
        m_SerializedUniforms[name].m_Texture = texture;
        m_SerializedUniforms[name].m_Active = true;
    }

    void Material::setCubemap(const char* name, Cubemap* cubemap)
    {
        m_SerializedUniforms[name].m_Cubemap = cubemap;
        m_SerializedUniforms[name].m_Active = true;
    }

    void Material::submitUniforms()
    {
        auto uniform = Engine::getInstance()->getUniform();

        for (const auto& [k, v] : m_SerializedUniforms)
        {
            if (!v.m_Active) continue;

            if (v.m_Type == ShaderUniformType::Texture)
                uniform->setTextureUniform(k, v.m_Texture, v.m_RegisterId);
            else if (v.m_Type == ShaderUniformType::Cubemap)
                uniform->setCubemapUniform(k, v.m_Cubemap, v.m_RegisterId);
        }
    }

    Material::Material() : m_Name("MaterialName")
    {
    }

    Material::~Material()
    {
    }
}
