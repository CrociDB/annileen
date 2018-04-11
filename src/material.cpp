#include "material.h"

void Material::init(Shader* shader)
{
    m_Shader = std::shared_ptr<Shader>(shader);
}

Shader* Material::getShader()
{
    return m_Shader.get();
}

Material::Material()
{
}

Material::~Material()
{
}
