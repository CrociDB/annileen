#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "shaderpass.h"
#include "asset.h"

namespace annileen
{
    class Texture;
    class Cubemap;

    struct MaterialSerializedUniform
    {
        uint8_t m_RegisterId;
        bool m_Active;
        ShaderUniformType m_Type;
        union
        {
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

        void submitUniforms();

        Material();
        ~Material();
    };
}
