#pragma once

#include <iostream>
#include <memory>
#include <map>

#include "shader.h"
#include "texture.h"
//#include "cubemap.h"

namespace annileen
{
    class Material
    {
    private:
        Shader* m_Shader;
        std::map<std::string, Texture*> m_Textures;
        //std::map<std::string, Cubemap*> m_Cubemaps;

    public:
        void init(Shader* shader);

        Shader* getShader();
        void addTexture(const char* name, Texture* texture);
        //void addCubemap(const char* name, Cubemap* cubemap);

        void submitTextures();

        Material();
        ~Material();
    };
}
