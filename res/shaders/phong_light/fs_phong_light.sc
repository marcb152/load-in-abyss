$input v_worldPos, v_normal

#include "../common.sh"

uniform vec4 u_lightPos;

void main()
{
    vec3 materialColor = vec3( 0.7f,0.7f,0.9f );
    vec3 ambient = vec3( 0.05f,0.05f,0.05f );
    vec3 diffuseColor = vec3( 1.0f,1.0f,1.0f );
    float diffuseIntensity = 2.0f;
    float attConst = 1.0f;
    float attLin = 0.045f;
    float attQuad = 0.0075f;

	// fragment to light vector data
    vec3 vToL = u_lightPos.xyz - v_worldPos;
    float distToL = length( vToL );
    vec3 dirToL = vToL / distToL;

    // diffuse attenuation
    float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
 
    // diffuse intensity
    vec3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, v_normal));
 
    // final color
    gl_FragColor = vec4(saturate((diffuse + ambient) * materialColor), 1.0f);
    //gl_FragColor = vec4(v_normal, 1.0);
}
