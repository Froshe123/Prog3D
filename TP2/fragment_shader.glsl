#version 330 core
//Uniforms
uniform vec3 u_light1Orientation;
uniform vec3 u_light1Ambient;
uniform bool u_enableLight1;

// Output data
out vec4 FragColor;
// Ajouter une variable interpolée o_color venant du vertex shader
//

void main()
{
    // Mettre à jour la couleur avec la variable interpolée venant du vertex shader
    FragColor = vec4(1., 0., 0., 1.) * vec4(u_light1Ambient, 1.0);// Output color = red

}
