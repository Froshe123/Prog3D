#version 330 core

uniform vec3 viewVector;
uniform float time;

in vec2 uv;
in vec3 position;
in vec3 face_normal;

out vec4 color;

// Question 2.3.1
// Pareil que précédemment, importez votre texture et échantillonnez la. 
// la subtilité est unpeu après dans la question
uniform sampler2D albedoTexture;

void main() {
    // Question 2.3.1
    color = texture(albedoTexture,uv);
}