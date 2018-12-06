#pragma once

#include "skybox.h"

void Skybox::createMesh()
{
    Shader* shader = new Shader();
    shader->load("../../shaders/skybox.vert", "../../shaders/skybox.frag");

    Material* material = new Material();
    material->init(shader);
    material->addCubemap("skybox", m_Cubemap);
    material->loadTextures();

    m_Mesh = new Mesh();
    m_Mesh->init(skyboxVertices, sizeof(skyboxVertices) / sizeof(float), 0);
    m_Mesh->setMaterial(material);
    m_Mesh->genBuffers();
}

Mesh* Skybox::getMesh() const
{
    return m_Mesh;
}

void Skybox::destroyMesh()
{
    delete m_Mesh;
}

Skybox::Skybox(Cubemap* cubemap)
{
    m_Cubemap = cubemap;
}

Skybox::~Skybox()
{
    destroyMesh();
}
