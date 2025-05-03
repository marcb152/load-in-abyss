$input a_position, a_normal
$output v_worldPos, v_normal

#include "../common.sh"

void main()
{
    vec3 worldPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
	v_worldPos = worldPos;
	vec3 normal = a_normal.xyz;
    vec3 worldNormal = mul(u_model[0], vec4(normal.xyz, 0.0)).xyz;
    v_normal = normalize(worldNormal);
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
}
