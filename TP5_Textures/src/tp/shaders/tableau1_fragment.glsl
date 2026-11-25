#version 330 core

uniform vec3 viewVector;
uniform float time;

// Question 2.1
// Importez les UV ici
in vec2 uv;
in vec3 position;
in vec3 face_normal;

out vec4 color;


void main() {
    // Question 2.1
    // Mettez les uv calculés dans la couleur, par exemple dans les deux première composantes.
    color = vec4(uv, 0.0, 1.0); // u -> rouge, v -> vert

}