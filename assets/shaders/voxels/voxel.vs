$input a_position, a_normal, a_texcoord0
$output v_position, v_texcoord0, v_normal
 
#include <bgfx_shader.sh>

void main()
{
	v_position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_normal = a_normal;
	v_texcoord0 = a_texcoord0;
	
	gl_Position = v_position;
}
