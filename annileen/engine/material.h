#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <map>

namespace annileen
{
    class ShaderPass;
    class Texture;
    class Cubemap;

    class Material
    {
    private:
        
        std::vector<std::shared_ptr<ShaderPass>> m_ShaderPasses;
        std::map<std::string, std::pair<uint8_t,Texture*>> m_Textures;
        std::map<std::string, std::pair<uint8_t,Cubemap*>> m_Cubemaps;

    public:
        void addShaderPass(std::shared_ptr<ShaderPass> shaderPass);
        void removeShaderPass(std::shared_ptr<ShaderPass> shaderPass);
        std::shared_ptr<ShaderPass> getShaderPassAt(size_t shaderPassId);
        size_t getNumberOfShaderPasses() const;

        void addTexture(const char* name, Texture* texture, uint8_t registerId);
        void addCubemap(const char* name, Cubemap* cubemap, uint8_t registerId);

        void submitUniforms();

        Material();
        ~Material();
    };
}
