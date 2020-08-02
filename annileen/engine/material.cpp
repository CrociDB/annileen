#include "material.h"
#include "engine.h"

namespace annileen
{
    void Material::init(Shader* shader)
    {
        m_Shader = shader;
    }

    Shader* Material::getShader()
    {
        return m_Shader;
    }

    void Material::addTexture(const char* name, Texture* texture)
    {
        m_Textures[name] = texture;
    }

    void Material::addCubemap(const char* name, Cubemap* cubemap)
    {
        m_Cubemaps[name] = cubemap;
    }

    void Material::submitUniforms()
    {
        auto uniform = Engine::getInstance()->getUniform();

        for (const auto& [k, v] : m_Textures)
        {
            uniform->setTextureUniform(k, v);
        }

        for (const auto& [k, v] : m_Cubemaps)
        {
            uniform->setCubemapUniform(k, v);
        }
    }

    Material::Material()
    {
    }

    Material::~Material()
    {
    }
}
