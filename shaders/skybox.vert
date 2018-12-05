#version 330 core

layout (location = 0) in vec3 position;

out vec3 frag_uv;

// transformation matrices
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * vec4(position, 1.0);
   frag_uv = position;
}
    