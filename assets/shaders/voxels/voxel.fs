$input v_position, v_texcoord0, v_normal

#include <bgfx_shader.sh>
#include "../default/annileen.sh"

SAMPLER2D(s_mainTex,  0);


void main()
{
	const float light_specular_strength = 0.3f;
	const float light_specular_power = 16.0f;

	vec3 normal = v_normal;
	normal.x = -normal.x;
	normal = normalize(normal);

	vec4 tex = texture2D(s_mainTex, v_texcoord0);
	vec3 ambient = 0.4 * vec3(1.0, 1.0, 1.0);

	vec3 viewDist = (u_viewPos - v_position.xyz);
	vec3 viewDir = normalize(viewDist);

	float diff = max(dot(normal, u_lightDirection.xyz), 0.0);
	vec3 diffuse = diff * u_lightColor.xyz * u_lightIntensity.x;

	vec3 reflectDir = reflect(-u_lightDirection.xyz, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), light_specular_power);
	vec3 specular = light_specular_strength * spec * u_lightColor.xyz;

	vec3 a = ambient * tex.xyz;
	vec3 d = diffuse * tex.xyz;
	vec3 finalColor = a + d + specular;

	gl_FragColor = vec4(finalColor, 1.0);
}