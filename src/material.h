#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <iostream>
#include <memory>
#include <map>

#include "shader.h"
#include "texture.h"
#include "cubemap.h"

class Material
{
private:
    Shader* m_Shader;
    std::map<std::string, Texture> m_Textures;
    std::map<std::string, Cubemap*> m_Cubemaps;

public:
    void init(Shader* shader);

    Shader* getShader();
    void addTexture(const char* name, Texture texture);
    void addCubemap(const char* name, Cubemap* cubemap);
    void loadTextures();
    void useTextures();

    Material();
    ~Material();
};

#endif

