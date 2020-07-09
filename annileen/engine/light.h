#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm.hpp>

#include "transform.h"

namespace annileen
{
    enum LightType
    {
        LightDirectional
    };

    class Light
    {
    public:
        Transform transform;
        glm::vec3 color;
        LightType type;
        float intensity;
    };
}

#endif
