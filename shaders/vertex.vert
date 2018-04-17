#version 330 core

layout (location = 0) in vec3 position;
in vec3 color;
in vec2 uv;

out vec3 frag_color;

void main()
{
   gl_Position = vec4(position, 1.0);
   frag_color = color;
}
