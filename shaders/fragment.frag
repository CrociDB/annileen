#version 330 core

in vec3 frag_color;
in vec2 frag_uv;

out vec4 color;

uniform sampler2D mainTex;

void main()
{
   color = texture(mainTex, frag_uv) * vec4(frag_color, 1.0f);
}