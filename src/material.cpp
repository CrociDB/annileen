#include "material.h"

void Material::init(Shader* shader)
{
    m_Shader = std::shared_ptr<Shader>(shader);
}

Material::Material()
{
}

Material::~Material()
{
}
