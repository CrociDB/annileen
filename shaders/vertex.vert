#version 330 core

layout (location = 0) in vec3 position;
in vec3 color;

out vec3 frag_color;

void main()
{
   gl_Position = vec4(position.x, position.y, position.z, 1.0);
   frag_color = color;
}