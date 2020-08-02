#pragma once

#include "cubemap.h"

const float s_skyboxCubeVertices[8][3] =
{
    {-1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f},
    {-1.0f,  1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
};

const uint16_t s_skyboxCubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

namespace annileen
{
    class Model;

    class Skybox
    {
    private:
        std::shared_ptr<Model> m_Model;
        Cubemap* m_Cubemap;
        void createModel();

    public:
        std::shared_ptr<Model> getModel() const { return m_Model; }
        Cubemap* getCubemap() const { return m_Cubemap; }

        Skybox(Cubemap* cubemap);
        ~Skybox();
    };
}
