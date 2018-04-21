#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <iostream>
#include <memory>
#include <map>

#include "shader.h"
#include "texture.h"

class Material
{
private:
    std::shared_ptr<Shader> m_Shader;
	std::map<std::string, Texture> m_Textures;

public:
    void init(Shader* shader);

    Shader* getShader();
	void addTexture(const char* name, Texture texture);
    void loadTextures();
    void useTextures();

    Material();
    ~Material();
};

#endif

