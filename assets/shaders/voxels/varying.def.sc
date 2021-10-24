vec2 v_texcoord0    : TEXCOORD0 = vec2(0.0, 0.0);
vec3 v_normal       : NORMAL    = vec3(0.0, 0.0, 1.0);
vec4 v_position     : TEXCOORD1;
vec4 v_shadowcoord  : TEXCOORD2 = vec4(0.0, 0.0, 0.0, 0.0);
vec3 v_view         : TEXCOORD3 = vec3(0.0, 0.0, 0.0);

vec3 a_position   : POSITION;
vec4 a_normal     : NORMAL;
vec4 a_tangent    : TANGENT;
vec2 a_texcoord0  : TEXCOORD0;
