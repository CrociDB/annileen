#version 330 core

layout (location = 0) in vec3 position;
in vec3 color;
in vec2 uv;

out vec3 frag_color;
out vec2 frag_uv;

// transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = model * vec4(position, 1.0);
   frag_color = color;
   frag_uv = vec2(uv.x, 1.0 - uv.y);
}
