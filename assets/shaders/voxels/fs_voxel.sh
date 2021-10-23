SAMPLER2D(s_mainTex,  0);

#if SHADOW_ENABLED
//#define SHADOW_PACKED_DEPTH 0

//#if SHADOW_PACKED_DEPTH
//SAMPLER2D(s_shadowMap, 0);
//#define Sampler sampler2D
//#else
SAMPLER2DSHADOW(s_shadowMap, 1);
#define Sampler sampler2DShadow
//#endif // SHADOW_PACKED_DEPTH

float hardShadow(Sampler _sampler, vec4 _shadowCoord, float _bias)
{
	vec3 texCoord = _shadowCoord.xyz/_shadowCoord.w;
//#if SHADOW_PACKED_DEPTH
//	return step(texCoord.z-_bias, unpackRgbaToFloat(texture2D(_sampler, texCoord.xy) ) );
//#else
	return shadow2D(_sampler, vec3(texCoord.xy, texCoord.z-_bias) );
//#endif // SHADOW_PACKED_DEPTH
}

float PCF(Sampler _sampler, vec4 _shadowCoord, float _bias, vec2 _texelSize)
{
	vec2 texCoord = _shadowCoord.xy/_shadowCoord.w;

	bool outside = any(greaterThan(texCoord, vec2_splat(1.0)))
				|| any(lessThan   (texCoord, vec2_splat(0.0)))
				 ;

	if (outside)
	{
		return 1.0;
	}

	float result = 0.0;
	vec2 offset = _texelSize * _shadowCoord.w;

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-1.5,  1.5) * offset, 0.0, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(-0.5,  1.5) * offset, 0.0, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(0.5,  1.5) * offset, 0.0, 0.0), _bias);

	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5, -1.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5, -0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5,  0.5) * offset, 0.0, 0.0), _bias);
	result += hardShadow(_sampler, _shadowCoord + vec4(vec2(1.5,  1.5) * offset, 0.0, 0.0), _bias);

	return result / 16.0;
}
#endif

void main()
{
	const float light_specular_strength = 0.3f;
	const float light_specular_power = 16.0f;

	vec3 normal = v_normal;
	normal.x = -normal.x;
	normal = normalize(normal);

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

	#if SHADOW_ENABLED
	// shadow
	float shadowMapBias = 0.001;
	vec2 texelSize = vec2_splat(1.0/1024.0);
	float visibility = PCF(s_shadowMap, v_shadowcoord, shadowMapBias, texelSize);
	finalColor = a + (d + specular) * visibility;
	#endif

	finalColor = mix(finalColor, u_fogColor.xyz, clamp(pow(length(viewDist) / u_fogSettings.x, u_fogSettings.y), 0.0, 1.0) * u_fogSettings.z);

	float gamma = 2.2;
	finalColor.rgb = finalColor.rgb / (finalColor.rgb + vec3(1.0));
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0/gamma));

	gl_FragColor = vec4(finalColor, 1.0);
}