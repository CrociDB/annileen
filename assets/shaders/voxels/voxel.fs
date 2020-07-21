$input v_texcoord0, v_normal

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>

SAMPLER2D(s_mainTex,  0);

void main()
{
	//gl_FragColor = vec4(v_normal, 1.0);
	gl_FragColor = texture2D(s_mainTex, v_texcoord0);
}
