$input v_texcoord

/*
 * Copyright 2014-2016 Dario Manesku. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>
#include "../default/annileen.sh"

SAMPLERCUBE(s_texCube, 0);
//SAMPLER2D(s_texCube, 0);

void main()
{
	gl_FragColor = textureCube(s_texCube, v_texcoord);
}
