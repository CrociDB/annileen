#include "material.h"

void Material::init(Shader* shader)
{
    m_Shader = shader;
}

Shader* Material::getShader()
{
    return m_Shader;
}

void Material::addTexture(const char* name, Texture texture)
{
    m_Textures.insert(std::pair<std::string, Texture>(std::string(name), texture));
}

void Material::addCubemap(const char* name, Cubemap* cubemap)
{
    m_Cubemaps.insert(std::pair<std::string, Cubemap*>(std::string(name), cubemap));
}

void Material::loadTextures()
{
	for (auto& tex : m_Textures)
	{
		tex.second.load();
	}

    for (auto& cube : m_Cubemaps)
    {
        cube.second->load();
    }
}

void Material::useTextures()
{
    uint8_t texCount = 0;
    for (auto& tex : m_Textures)
    {
        glActiveTexture(GL_TEXTURE0 + texCount);
        glBindTexture(GL_TEXTURE_2D, tex.second.getId());
        m_Shader->setInt(tex.first, texCount);
        texCount++;
    }

    for (auto& cube : m_Cubemaps)
    {
        glActiveTexture(GL_TEXTURE0 + texCount);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cube.second->getId());
        m_Shader->setInt(cube.first, texCount);
        texCount++;
    }
}

Material::Material()
{
}

Material::~Material()
{
}
