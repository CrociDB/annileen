#pragma once

#include <glm.hpp>

#include <engine/transform.h>

namespace annileen
{
    enum class LightType
    {
        Spot,
        Point,
        Directional
    };

    class Light
    {
    public:
        // Should this be moved to a parent component class? Will model be a "component"?
        bool isStatic = false;
        bool enabled = true;

        Transform transform;
        glm::vec3 color;
        LightType type;
        float intensity;
    };
}
