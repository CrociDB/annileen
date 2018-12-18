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

uniform float light_specular_strength = 0.4f;
uniform float light_specular_power = 16;

// fog
uniform vec3 fog_color = vec3(.3, .3, .3);
uniform float fog_dist = 30.0;
uniform float fog_pow = 2.0;
uniform float fog_enabled = 1.0;

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

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), light_specular_power);
   vec3 specular = light_specular_strength * spec * light_color;

   vec3 finalColor = (ambient + diffuse + specular) * tex.xyz;

   color = vec4(mix(finalColor, fog_color, clamp(pow(length(viewDist) / fog_dist, fog_pow), 0.0, 1.0) * fog_enabled), 1.0);
//   color = vec4(tex.xyz * vec3(.5) +  (0.3 * frag_normal), 1.0);
}