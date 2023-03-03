module;

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

export module anni.material;

import anni.shaderpass;
import anni.asset;
import anni.uniform;
import anni.cubemap;
import anni.texture;

export namespace annileen
{
    struct MaterialSerializedUniform
    {
        MaterialSerializedUniform() {}
        ~MaterialSerializedUniform() {}

        MaterialSerializedUniform& operator=(MaterialSerializedUniform& other)
        {
            if (this != &other)
            {
                using std::swap;

                swap(registerId, other.registerId);
                swap(active, other.active);
                swap(type, other.type);
                swap(data, other.data);
            }

            return *this;
        }

        uint8_t registerId{ 0 };
        bool active{ false };
        ShaderUniformType type{ ShaderUniformType::Texture };
        union
        {
            std::shared_ptr<void> data{ nullptr };
            std::shared_ptr<Texture> texture;
            std::shared_ptr<Cubemap> cubemap;
        };
    };

    class Material
    {
    public:
        Material() = default;
        ~Material();

    public:
        void addShaderPass(std::shared_ptr<ShaderPass> shaderPass);
        void removeShaderPass(std::shared_ptr<ShaderPass> shaderPass);
        std::shared_ptr<ShaderPass> getShaderPassAt(size_t shaderPassId);
        size_t getNumberOfShaderPasses() const;

        void setName(const std::string& name);
        const std::string& getName() const;

        // Creating Uniforms
        void addTexture(const char* name, std::shared_ptr<Texture> texture, uint8_t registerId, bool active);
        void addCubemap(const char* name, std::shared_ptr<Cubemap> cubemap, uint8_t registerId, bool active);

        // Setting Uniforms
        void setTexture(const char* name, std::shared_ptr<Texture> texture);
        void setCubemap(const char* name, std::shared_ptr<Cubemap> cubemap);

        std::map<std::string, MaterialSerializedUniform>& getSerializedUniforms();

        void submitUniforms();
    
    private:
        std::string m_Name{ "MaterialName" };
        std::vector<std::shared_ptr<ShaderPass>> m_ShaderPasses{};
        std::map<std::string, MaterialSerializedUniform> m_SerializedUniforms{};
    };
}

module :private;

namespace annileen
{
    Material::~Material()
    {
        // TODO: remove
        std::cout << "Material " << m_Name <<" destroyed." << std::endl;
    }

    void Material::setName(const std::string& name) 
    { 
        m_Name = name; 
    }

    const std::string& Material::getName() const 
    { 
        return m_Name; 
    }

    std::map<std::string, MaterialSerializedUniform>& Material::getSerializedUniforms()
    {
        return m_SerializedUniforms;
    }

    void Material::addShaderPass(std::shared_ptr<ShaderPass> shaderPass)
    {
        m_ShaderPasses.push_back(shaderPass);

        auto& shaderAvailableUniforms{ shaderPass->getShaderAvailableUniforms() };
        for (const auto& availableUniform : shaderAvailableUniforms)
        {
            addTexture(availableUniform.m_UniformName.c_str(), nullptr, availableUniform.m_Position, false);
        }
    }
    
    void Material::removeShaderPass(std::shared_ptr<ShaderPass> shaderPass)
    {
        auto shaderPassIt{ std::find(m_ShaderPasses.begin(), m_ShaderPasses.end(), shaderPass) };
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

    void Material::addTexture(const char* name, std::shared_ptr<Texture> texture, uint8_t registerId, bool active)
    {
        MaterialSerializedUniform serializedUniform;
        serializedUniform.registerId = registerId;
        serializedUniform.type = ShaderUniformType::Texture;
        serializedUniform.texture = texture;
        serializedUniform.active = active;
        m_SerializedUniforms[name] = serializedUniform;
    }

    void Material::addCubemap(const char* name, std::shared_ptr<Cubemap> cubemap, uint8_t registerId, bool active)
    {
        MaterialSerializedUniform serializedUniform;
        serializedUniform.registerId = registerId;
        serializedUniform.type = ShaderUniformType::Texture;
        serializedUniform.cubemap = cubemap;
        serializedUniform.active = active;
        m_SerializedUniforms[name] = serializedUniform;
    }

    void Material::setTexture(const char* name, std::shared_ptr<Texture> texture)
    {
        m_SerializedUniforms[name].texture = texture;
        m_SerializedUniforms[name].active = true;
    }

    void Material::setCubemap(const char* name, std::shared_ptr<Cubemap> cubemap)
    {
        m_SerializedUniforms[name].cubemap = cubemap;
        m_SerializedUniforms[name].active = true;
    }

    void Material::submitUniforms()
    {
        for (const auto& [k, v] : m_SerializedUniforms)
        {
            if (!v.active || v.data == nullptr) continue;

            if (v.type == ShaderUniformType::Texture)
                Uniform::setTextureUniform(k, v.texture, v.registerId);
            else if (v.type == ShaderUniformType::Cubemap)
                Uniform::setCubemapUniform(k, v.cubemap, v.registerId);
        }
    }
}
