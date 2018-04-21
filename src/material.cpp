#include "material.h"

void Material::init(Shader* shader)
{
    m_Shader = std::shared_ptr<Shader>(shader);
}

Shader* Material::getShader()
{
    return m_Shader.get();
}

void Material::addTexture(const char* name, Texture texture)
{
	m_Textures.insert(std::pair<std::string, Texture>(std::string(name), texture));
}

void Material::loadTextures()
{
	for (auto& tex : m_Textures)
	{
		tex.second.load();
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
}

Material::Material()
{
}

Material::~Material()
{
}
