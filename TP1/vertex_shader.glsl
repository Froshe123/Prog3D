#version 330 core
// TODO
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle
// i.e. a appliquer sur tous les sommets
//---------
uniform mat4 u_mvpMatrix;

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shaderx

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

//------

void main(){

    //Mettre à jour la ligne pour appliquer la translation et la mise à l'échelle mise à jour par le programme
    gl_Position = vec4((u_mvpMatrix*vec4(position, 0)).xyz, 1);

    //TODO : assign the normal to the interpolated variable color
}

