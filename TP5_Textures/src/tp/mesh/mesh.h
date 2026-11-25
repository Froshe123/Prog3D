#pragma once


#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <vector>
#include <common/shader.hpp>
#include <map>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include "texture/texture.h"
#include "camera/camera.h"

// le type d'entier que l'on utilise pour indexer les points dans les triangles.
using TRI_IDX_TYPE = unsigned int;
extern GLuint TRI_GL_TYPE; // défini dans le cpp.

struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (TRI_IDX_TYPE v0, TRI_IDX_TYPE v1, TRI_IDX_TYPE v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    TRI_IDX_TYPE & operator [] (unsigned int iv) { return v[iv]; }
    TRI_IDX_TYPE operator [] (unsigned int iv) const { return v[iv]; }
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    std::array<TRI_IDX_TYPE, 3> v;
};


class Mesh {
protected:
    void unsynchronize() const;
    void synchronize() const;

    // les variables mutables sont modifiables dans les fonctions définies comme const.
    // Ici, on s'en sert pour modifier les identifiants des buffers GPU.
    // La logique est que même une fonction ne touchant pas à l'objet C++ peut toucher aux buffers gpu
    mutable bool _synchronized = false;
    mutable GLuint _VBO;
    mutable GLuint _VAO; // https://stackoverflow.com/questions/21652546/what-is-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relatio
    mutable GLuint _EBO;
    mutable GLuint _UV;
    mutable GLuint _NORMALS;
    mutable GLuint _TANGENTS;
    mutable GLuint _BITANGENTS;

public:

    GLuint shaderPID;
    glm::mat4 transform;
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;

    std::vector< glm::vec3 > normals;
    std::vector< glm::vec3 > tangents;
    std::vector< glm::vec3 > bitangents;

    std::vector< Triangle > triangles;

    Mesh() = default;

    Mesh(std::string path_model, std::string path_shader_vert, std::string path_shader_frag); // défini dans objLoader

    ~Mesh(){ // On libère les buffers sur la carte graphique à la destruction de l'objet.
        if (_synchronized){
            unsynchronize();
        }
    }

    void recomputeNormals();

    void recomputeTangents();

    void setShader(std::string vertex_shader, std::string fragment_shader);

    void render(const Camera& camera) const;

    // pour pouvoir print les caractéristiques d'un mesh
    friend std::ostream& operator<< (std::ostream& stream, const Mesh& mesh) {
            stream << "Mesh(" << mesh.vertices.size() <<" verts, " << mesh.triangles.size() <<" tris)";
        return stream;
    }
    // Question 2.2
    // Ajouter un attribut pour mettre votre texture
    // La fonction suivante est là pour vous donner une piste, vous n'êtes pas obligé de l'utiliser.
    std::map<std::string, Texture> textures;

    void addTexture(const std::string & name, const Texture & texture){
        // Dans le cas d'une std::map reliant les couples " nom de la texture dans leshader" - texture
        // Vous pouvez aussi utiliser un nomfixe dans le shader (ex "texture"), et stocker
        // la texture directement dans un attribut ici.
        textures.emplace(name, texture);
    }
};