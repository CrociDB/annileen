SAMPLER2D(s_mainTex,  0);
SAMPLER2D(s_mainNormal,  2);

void main()
{
	const float light_specular_strength = 0.3f;
	const float light_specular_power = 16.0f;

	vec3 normal = v_normal;
	normal.x = -normal.x;
	normal = normalize(normal);

	mat3 tbn = mat3(v_tangent, v_bitangent, normal);
	normal = texture2D(s_mainNormal, v_texcoord0).rgb;
	normal = normalize(tbn * normal);

	vec4 tex = texture2D(s_mainTex, v_texcoord0);
	vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);

	vec3 viewDist = (u_viewPos - v_position.xyz);
	vec3 viewDir = normalize(viewDist);

	float diff = max(dot(normal, u_lightDirection.xyz), 0.0);
	vec3 diffuse = diff * u_lightColor.xyz * u_lightIntensity.x;

	vec3 reflectDir = reflect(-u_lightDirection.xyz, normal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), light_specular_power);
	vec3 specular = light_specular_strength * spec * u_lightColor.xyz;


	vec3 a = ambient * tex.xyz;
	vec3 d = diffuse * tex.xyz;
	vec3 finalColor = a + (d + specular);

	// Shadows
	#if SHADOW_ENABLED
	float shadowMapBias = 0.001;
	vec2 texelSize = vec2_splat(1.0/2048.0);
	float visibility = _annileen_PCF(s_shadowMap, v_shadowcoord, shadowMapBias, texelSize);
	finalColor = a + (d + specular) * visibility;
	#endif

	finalColor = mix(finalColor, u_fogColor.xyz, clamp(pow(length(viewDist) / u_fogSettings.x, u_fogSettings.y), 0.0, 1.0) * u_fogSettings.z);

	float gamma = 2.2;
	finalColor.rgb = finalColor.rgb / (finalColor.rgb + vec3(1.0));
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0/gamma));

	gl_FragColor = vec4(finalColor, 1.0);
}