#version 330 core

in vec3 frag_position;
in vec2 frag_uv;
in vec3 frag_normal;

out vec4 color;

uniform sampler2D mainTex;

// light
uniform vec3 light_direction;
uniform vec3 light_color;
uniform float light_intensity;

uniform vec3 view_position;

void main()
{
   vec4 tex = texture(mainTex, frag_uv);
   vec3 ambient = 0.4f * vec3(1.0);

   float diff = max(dot(frag_normal, light_direction), 0.0);
   vec3 diffuse = diff * light_color;

   vec3 viewDist = (view_position - frag_position);
   vec3 viewDir = normalize(viewDist);
   vec3 reflectDir = reflect(-light_direction, frag_normal);

   float specularStrength = 0.4f;
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
   vec3 specular = specularStrength * spec * light_color;

   color = vec4((ambient + diffuse + specular) * tex.xyz, 1.0);
}