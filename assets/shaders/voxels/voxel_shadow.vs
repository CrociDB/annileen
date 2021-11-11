$input a_position, a_normal, a_tangent, a_texcoord0
$output v_position, v_texcoord0, v_normal, v_tangent, v_bitangent, v_shadowcoord, v_view
 
#include <bgfx_shader.sh>

#define SHADOW_ENABLED 1
#include "vs_voxel.sh"
