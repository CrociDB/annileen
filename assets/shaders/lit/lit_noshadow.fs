$input v_position, v_texcoord0, v_normal, v_tangent, v_bitangent, v_shadowcoord, v_view

#include <bgfx_shader.sh>

#define SHADOW_ENABLED 0

#include "../default/annileen.sh"
#include "../default/annileen_utils.sh"
#include "fs_lit.sh"