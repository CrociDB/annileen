$input a_position, a_normal, a_texcoord0
$output v_position, v_texcoord0, v_normal, v_shadowcoord, v_view
 
#include <bgfx_shader.sh>

uniform mat4 u_lightMtx;

void main()
{
	v_position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	
	vec4 normal = a_normal;// * 2.0 - 1.0;
	v_normal = normalize(mul(u_modelView, vec4(normal.xyz, 0.0) ).xyz);
	
	v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;

	const float shadowMapOffset = 0.001;
	vec3 posOffset = a_position + normal.xyz * shadowMapOffset;
	v_shadowcoord = mul(u_lightMtx, vec4(posOffset, 1.0) );

	v_texcoord0 = a_texcoord0;	
	
	gl_Position = v_position;

	//v_normal = a_normal;
}

