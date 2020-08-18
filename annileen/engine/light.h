#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm.hpp>

#include "transform.h"

namespace annileen
{
    enum LightType
    {
        Spot,
        Point,
        Directional
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
