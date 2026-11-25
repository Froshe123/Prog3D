#version 330 core

uniform vec3 viewVector;
uniform float time;

in vec2 uv;
in vec3 position;
in vec3 face_normal;

out vec4 color;

// Question 2.2 
// Créez une variable uniforme de type sampler2D qui permettra d'échantilloner votre texture
uniform sampler2D albedoTexture;


void main() {

    // Question 2.2 
    // échantillonez votre texture en vos coordonnées uv grâce à la fonction texture()
    color = texture ( albedoTexture, uv);
}