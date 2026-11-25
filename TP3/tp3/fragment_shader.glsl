#version 330 core
uniform vec3 u_light1Orientation;
uniform vec3 u_light1Ambient;
uniform vec3 u_light1Diffuse;
uniform vec3 u_light1Specular;
uniform bool u_light1Enabled;

uniform mat4 u_viewMatrix;
uniform vec3 u_materialAmbient;
uniform vec3 u_materialDiffuse;
uniform vec3 u_materialSpecular;
uniform float u_materialShininess;

in vec3 o_color;      
in vec3 o_position;   

out vec4 FragColor;

void main()
{
    vec3 N = normalize(o_color);                                        
    vec3 L = normalize(-(mat3(u_viewMatrix) * u_light1Orientation));    
    vec3 V = normalize(-o_position);                                    
    vec3 R = 2.0 * dot(N, L) * N - L;                                   // R = 2(N.L)N - L

    vec3 Ia = u_light1Ambient * u_materialAmbient;                                          // Ia = Isa * Ka
    vec3 Id = u_light1Diffuse * max(dot(L, N), 0.0)* u_materialDiffuse ;                    // Id = Isd * Kd * (L.N)
    vec3 Is = u_light1Specular * pow(max(dot(R, V), 0.0), u_materialShininess) * u_materialSpecular;   // Is = Iss * Ks * (R.V)^n  

    vec3 I = Ia;
    if (u_light1Enabled)
        I = I + Id + Is;                                                // I = Ia + Id + Is

    FragColor = vec4(I, 1.0);
}