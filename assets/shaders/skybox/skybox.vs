$input a_position
$output v_texcoord

/*
 * Copyright 2014-2016 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>
#include "../default/annileen.sh"

void main()
{
	v_texcoord = a_position;
	vec4 pos = mul(u_viewProj, vec4(a_position, 1.0));
	gl_Position = pos.xyww;
}
