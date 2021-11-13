#if SHADOW_ENABLED
uniform mat4 u_lightMtx;
#endif

void main()
{
	v_position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	
	v_normal = normalize(mul(u_modelView, vec4(a_normal.xyz, 0.0) ).xyz);
	v_tangent = normalize(mul(u_modelView, vec4(a_tangent.xyz, 0.0) ).xyz);
	v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
	v_bitangent = cross(v_normal, v_tangent);

	v_view = mul(u_modelView, vec4(a_position, 1.0)).xyz;

#if SHADOW_ENABLED
	const float shadowMapOffset = 0.001;
	vec3 posOffset = a_position + v_normal.xyz * shadowMapOffset;
	v_shadowcoord = mul(u_lightMtx, vec4(posOffset, 1.0) );
#endif

	v_texcoord0 = a_texcoord0;	
	
	gl_Position = v_position;
}

