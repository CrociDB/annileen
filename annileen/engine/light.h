#pragma once

#include <glm.hpp>

#include <engine/transform.h>
#include <engine/scenenodemodule.h>

namespace annileen
{
    enum class LightType
    {
        Spot,
        Point,
        Directional
    };

    class Light final : public SceneNodeModule
    {
    public:
        // Should this be moved to a parent component class? Will model be a "component"?
        bool isStatic = false;
        bool enabled = true;
        bool generateShadows = true;

        glm::vec3 color;
        LightType type;
        float intensity;
    };

    typedef Light* LightPtr;
}
