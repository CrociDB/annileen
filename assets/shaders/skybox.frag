#version 330 core

in vec3 frag_uv;

out vec4 color;

uniform samplerCube skybox;

void main()
{
   color = texture(skybox, frag_uv);
}