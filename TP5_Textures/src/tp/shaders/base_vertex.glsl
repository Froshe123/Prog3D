#version 330 core

layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec3 mesh_normals;

// Question 2.1
// Récupérer les données d'UV dans le bon attribut
layout(location = 2) in vec2 mesh_uvs;

layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;

uniform mat4 MVP;
uniform mat4 modelMatrix; // passée en temps que matrice 3x3 par simplicité, car utilisé que pour les normales

// Question 2.1
// Transférer les uv au fragment shader par une variable "out"
out vec2 uv;

out vec3 position;
out vec3 face_normal;
out mat3 TBN;

void main(){

        face_normal = mesh_normals;
        uv = mesh_uvs; // interpolées par le rasteriseur pour chaque fragment
        vec4 pos = vec4(vertices_position_modelspace,1);

        gl_Position = MVP * pos;
        position = (modelMatrix * pos).xyz;
        
        // Question 3.2
        // Calculer la matrice3 de la TBN. Les vecteurs qui la composent sont la tangente, la bitangente et la normale.
        mat3 model3 = mat3(modelMatrix);
        vec3 T = normalize(model3 * tangents);
        vec3 B = normalize(model3 * bitangents);
        vec3 N = normalize(model3 * mesh_normals);
        TBN = mat3(T, B, N);
}