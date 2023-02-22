module;

#include <glm.hpp>

export module light;

import transform;
import scenenode;

export namespace annileen
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
}
