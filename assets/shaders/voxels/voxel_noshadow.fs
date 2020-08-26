$input v_position, v_texcoord0, v_normal, v_view, v_shadowcoord

#include <bgfx_shader.sh>
#include "../default/annileen.sh"

#define SHADOW_ENABLED 0
#include "fs_voxel.sh"