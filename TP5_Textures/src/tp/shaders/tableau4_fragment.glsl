#version 330 core

out vec4 color;


uniform vec3 viewVector;
uniform float time;

in vec2 uv;
in vec3 position;
in vec3 face_normal;

// Question 3.3
// Importez votre TBN (dans une variable in) et vos deux textures ici (avec des sampler2D)
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
in mat3 TBN;

float time_speed = 1.0;
vec3 light_direction = normalize(vec3(2 * sin(time * time_speed), -2 *cos(time * time_speed), 1.0));
const vec4 light_color_power = vec4(1, 1, 0.7, 2.0);

vec3 normal = face_normal;
vec3 ambient = vec3(0.1, 0.1, 0.1);
vec3 albedo = vec3(1.0, 0.0, 0.0);
void main() {
    // Question 3.3
    // Récupérer la TBN calculée précédemment.
    // Echantillonez la normale map (en utilisant les mêmes uv) et transformez la à l'aide de la TBN. Utilisez cette nouvelle normale pour les calculs de lumière
    // Ne pas oublier de remplacer la couleur d'albedo en échantillonant la texture d'albedo
    albedo = texture(albedoTexture, uv).rgb;
    vec3 n_tangent = texture(normalTexture, uv).rgb * 2.0 - 1.0;
    normal = normalize(TBN * n_tangent);
    //Cosine theta diffuse lambertian component.
    float cosTheta = max(0.0, dot(normal, -light_direction));
    
    //Only if light is visible from the surface point.
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    if(cosTheta > 0.0) {
        //Reflection vector around normal.
        vec3 reflection_direction = reflect(light_direction, normal);
        
        //Diffuse component.
        diffuse = albedo * light_color_power.xyz * light_color_power.w * cosTheta;
        
        //Specular component.
        specular = light_color_power.xyz * pow(max(0.0, dot(reflection_direction, viewVector)), 2.0);
    }
    color.xyz = ambient + diffuse + specular;
}