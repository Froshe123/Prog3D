#version 330 core

uniform vec3 viewVector;
uniform float time;

in vec2 uv;
in vec3 position;
in vec3 face_normal;

out vec4 color;

float time_speed = 1.0;

vec3 light_positions[5] = vec3[](
    vec3(0.0, 100, 1.0),
    vec3(0.0, 100, 20.0),
    vec3(0.0, 100, 40.0),
    vec3(0.0, 100, 70.0),
    vec3(0.0, 100, 100.0)
);
const vec4 light_color_power = vec4(1, 1, 1.0, 2000);

vec3 normal = face_normal;
vec3 ambient = vec3(0.1, 0.1, 0.1);
vec3 albedo = vec3(0.8, 0.8, 0.8);
void main() {
    color.xyz = ambient;
    for (int light_index = 0; light_index < 5; ++light_index){
        float light_distance = distance(light_positions[light_index], position);
        vec3 light_direction = (position - light_positions[light_index]) / light_distance;

        float cosTheta = max(0.0, dot(normal, -light_direction));
        vec3 diffuse;
        vec3 specular;
        if(cosTheta > 0.0) { //Only if light is visible from the surface point.
            //Reflection vector around normal.
            vec3 reflection_direction = reflect(light_direction, normal);
            
            //Diffuse component.
            diffuse = albedo * light_color_power.xyz * light_color_power.w * cosTheta;
            
            //Specular component.
            specular = light_color_power.xyz * pow(max(0.0, dot(reflection_direction, viewVector)), 2.0);
        }
        color.xyz += (diffuse + specular) / (light_distance * light_distance) ;
    }
}