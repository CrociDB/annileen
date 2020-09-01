#include <engine/material.h>
#include <engine/engine.h>

//#include <engine/shaderpass.h>
//#include <engine/texture.h>
//#include <engine/cubemap.h>


namespace annileen
{
    void Material::addShaderPass(std::shared_ptr<ShaderPass> shaderPass)
    {
        m_ShaderPasses.push_back(shaderPass);
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

    void Material::addTexture(const char* name, Texture* texture, uint8_t registerId)
    {
        m_Textures[name] = std::make_pair(registerId, texture);
    }

    void Material::addCubemap(const char* name, Cubemap* cubemap, uint8_t registerId)
    {
        m_Cubemaps[name] = std::make_pair(registerId, cubemap);
    }

    void Material::submitUniforms()
    {
        auto uniform = Engine::getInstance()->getUniform();

        for (const auto& [k, v] : m_Textures)
        {
            uniform->setTextureUniform(k, v.second, v.first);
        }

        for (const auto& [k, v] : m_Cubemaps)
        {
            uniform->setCubemapUniform(k, v.second, v.first);
        }
    }

    Material::Material()
    {
    }

    Material::~Material()
    {
    }
}
