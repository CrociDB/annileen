module;

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

export module material;

import shaderpass;
import asset;
import uniform;
import cubemap;
import texture;

export namespace annileen
{
    struct MaterialSerializedUniform
    {
        uint8_t m_RegisterId;
        bool m_Active;
        ShaderUniformType m_Type;
        union
        {
            void* m_Data;
            Texture* m_Texture;
            Cubemap* m_Cubemap;
        };
    };

    class Material
    {
    private:
        std::string m_Name;
        std::vector<std::shared_ptr<ShaderPass>> m_ShaderPasses;

        std::map<std::string, MaterialSerializedUniform> m_SerializedUniforms;

    public:
        void addShaderPass(std::shared_ptr<ShaderPass> shaderPass);
        void removeShaderPass(std::shared_ptr<ShaderPass> shaderPass);
        std::shared_ptr<ShaderPass> getShaderPassAt(size_t shaderPassId);
        size_t getNumberOfShaderPasses() const;

        void setName(std::string name) { m_Name = name; }
        std::string& getName() { return m_Name; }

        // Creating Uniforms
        void addTexture(const char* name, Texture* texture, uint8_t registerId, bool active);
        void addCubemap(const char* name, Cubemap* cubemap, uint8_t registerId, bool active);

        // Setting Uniforms
        void setTexture(const char* name, Texture* texture);
        void setCubemap(const char* name, Cubemap* cubemap);

        inline std::map<std::string, MaterialSerializedUniform>& getSerializedUniforms()
        {
            return m_SerializedUniforms;
        }

        void submitUniforms();

        Material();
        ~Material();
    };
}

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
        for (const auto& [k, v] : m_SerializedUniforms)
        {
            if (!v.m_Active || v.m_Data == nullptr) continue;

            if (v.m_Type == ShaderUniformType::Texture)
                Uniform::setTextureUniform(k, v.m_Texture, v.m_RegisterId);
            else if (v.m_Type == ShaderUniformType::Cubemap)
                Uniform::setCubemapUniform(k, v.m_Cubemap, v.m_RegisterId);
        }
    }

    Material::Material() : m_Name("MaterialName")
    {
    }

    Material::~Material()
    {
    }
}
