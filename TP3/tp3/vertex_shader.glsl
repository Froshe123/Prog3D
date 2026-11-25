#version 330 core
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;   // attribut 1 = la normale

out vec3 o_color;      // normale en espace camera
out vec3 o_position;   // position en espace camera

void main(){
    vec4 positionCamera = u_viewMatrix * u_modelMatrix * vec4(position, 1.0);
    gl_Position = u_projectionMatrix * positionCamera;

    o_position = positionCamera.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(u_viewMatrix * u_modelMatrix)));
    o_color = normalize(normalMatrix * color);
}