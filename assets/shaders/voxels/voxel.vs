$input a_position, a_normal, a_texcoord0
$output v_texcoord0, v_normal

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
 
#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_normal = a_normal;
	v_texcoord0 = a_texcoord0;
}