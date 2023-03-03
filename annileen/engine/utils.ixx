module;

#include <glm.hpp>

export module anni.utils;

export uint32_t convert_color_vec3_uint32(glm::vec3 col)
{
    return ((uint32_t)(col.x * 0xFF) << 24) + ((uint32_t)(col.y * 0xFF) << 16) + ((uint32_t)(col.z * 0xFF) << 8) + 0xFF;
}