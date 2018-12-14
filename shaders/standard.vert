#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

out vec3 frag_position;
out vec2 frag_uv;
out vec3 frag_normal;

// transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   frag_position = vec3(model * vec4(position, 1.0));
   frag_uv = uv;
   frag_normal = mat3(transpose(inverse(model))) * normal;

   gl_Position = projection * view * model * vec4(position, 1.0);
}
      