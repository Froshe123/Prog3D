#include "mesh.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <common/shader.hpp>

#include "utils/utils.h"
#include "camera/camera.h"

GLuint TRI_GL_TYPE = GL_UNSIGNED_INT; // change with TRI_IDX_TYPE in mesh.h!


void Mesh::synchronize() const {

    if (_synchronized){
        unsynchronize();
    }
    
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    // buffer contenant tous les sommets
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // buffer contenant tous les indices
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), &triangles[0] , GL_STATIC_DRAW);

    // buffer contenant toutes les normales
    glGenBuffers(1, &_NORMALS);
    glBindBuffer(GL_ARRAY_BUFFER, _NORMALS);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0] , GL_STATIC_DRAW);

    // Question 2.1
    // buffer contenant tous les UV

    // (générer le buffer, puis y mettre les données du vecteurs d'UVs)
    glGenBuffers(1, &_UV);
    glBindBuffer(GL_ARRAY_BUFFER, _UV);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);



    // buffer contenant toutes les tangentes
    glGenBuffers(1, &_TANGENTS);
    glBindBuffer(GL_ARRAY_BUFFER, _TANGENTS);
    glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0] , GL_STATIC_DRAW);

    // buffer contenant toutes les bitangentes
    glGenBuffers(1, &_BITANGENTS);
    glBindBuffer(GL_ARRAY_BUFFER, _BITANGENTS);
    glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0] , GL_STATIC_DRAW);

    int attributeIndex = 0;

    glEnableVertexAttribArray(attributeIndex);
    
    // Positions attribute buffer
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glVertexAttribPointer(
        attributeIndex,                  // attribute
        3,    // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,  // stride
        (void*)0            // array buffer offset
    );


    // normals
    attributeIndex = 1;
    glBindBuffer(GL_ARRAY_BUFFER, _NORMALS);
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(attributeIndex);

    // Question 2.1
    // (Ajouter le buffer des UV au VAO à l'index 2)

    // uv
    attributeIndex = 2;
    glBindBuffer(GL_ARRAY_BUFFER, _UV);
    glVertexAttribPointer(attributeIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0); // 2 composantes (u, v)
    glEnableVertexAttribArray(attributeIndex);

    // Ceux là servent pour les cartes des normales
    // tangents
    attributeIndex = 3;
    glBindBuffer(GL_ARRAY_BUFFER, _TANGENTS);
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(attributeIndex);

    // bitangents
    attributeIndex = 4;
    glBindBuffer(GL_ARRAY_BUFFER, _BITANGENTS);
    glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(attributeIndex);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

    glBindVertexArray(0);

    _synchronized = true;
}

void Mesh::render(const Camera& camera) const{

    if (vertices.empty()) return;
    if (!_synchronized){
        synchronize();
    }
    glBindVertexArray(_VAO);
    glUseProgram(shaderPID);

    // On transmet la matrice MVP en tant qu'uniform au shader
    GLuint mvpUniformLocation = glGetUniformLocation(shaderPID, "MVP");
    glm::mat4 MVP = camera.getVP() * transform; // = P . V . M
    glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &MVP[0][0]);

    GLuint modelUniformLocation = glGetUniformLocation(shaderPID, "modelMatrix");
    glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, &transform[0][0]);
    GLuint viewVectorUniformLocation = glGetUniformLocation(shaderPID, "viewVector");
    glm::vec3 forwardVector = camera.getForwardVector();
    glUniform3fv(mvpUniformLocation, 1, &forwardVector[0]);

    GLuint timeUniformLocation = glGetUniformLocation(shaderPID, "time");
    glUniform1f(timeUniformLocation, glfwGetTime());
    
    // Question 2.2
    // Récupérer l'emplacement de votre sampler en utilisant le nom de l'uniforme dans votre shader
    // avec glGetUniformLocation
    // Puis bind la texture (en appelant lafonction bind que vous avez remplite, avec l'indice du slot de texture)
    // Finalement, envoyez l'indice du slot au shader avec glUniform1i
    int slot = 0;
    for (const auto & [name, texture] : textures){
        texture.bind(slot);
        glUniform1i(glGetUniformLocation(shaderPID, name.c_str()), slot);
        ++slot;
    }
    
    glDrawElements(
                GL_TRIANGLES,        // mode
                triangles.size()*3,  // count
                TRI_GL_TYPE,         // type
                (void*)0             // element array buffer offset
                );
    
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::unsynchronize() const {
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteBuffers(1, &_UV);
    glDeleteBuffers(1, &_NORMALS);
    glDeleteProgram(shaderPID);
    glDeleteVertexArrays(1, &_VAO);
    
    _synchronized = false;
}

void Mesh::setShader(std::string vertex_shader, std::string fragment_shader){
    shaderPID = loadShadersFromFileGLSL(vertex_shader.c_str(), fragment_shader.c_str());
}


void Mesh::recomputeNormals () {
    normals.resize(vertices.size());
    for (unsigned int i = 0; i < vertices.size (); i++)
        normals[i] = glm::vec3(0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < triangles.size (); i++) {
        glm::vec3 e01 = vertices[triangles[i].v[1]] -  vertices[triangles[i].v[0]];
        glm::vec3 e02 = vertices[triangles[i].v[2]] -  vertices[triangles[i].v[0]];
        glm::vec3 n = glm::cross(e01, e02);
        n = glm::normalize(n);
        for (unsigned int j = 0; j < 3; j++)
            normals[triangles[i].v[j]] += n;
    }
    for (unsigned int i = 0; i < vertices.size (); i++)
        normals[i] = glm::normalize(normals[i]);
}

void Mesh::recomputeTangents(){
    // https://terathon.com/blog/tangent-space.html
    std::vector<glm::vec3> tan1(vertices.size(), glm::vec3(0.0));
    std::vector<glm::vec3> tan2(vertices.size(), glm::vec3(0.0));
    
    for (long a = 0; a < triangles.size(); a++)
    {
        const Triangle &triangle = triangles[a];

        TRI_IDX_TYPE i1 = triangle[0];
        TRI_IDX_TYPE i2 = triangle[1];
        TRI_IDX_TYPE i3 = triangle[2];
        
        const glm::vec3 &v1 = vertices[i1];
        const glm::vec3 &v2 = vertices[i2];
        const glm::vec3 &v3 = vertices[i3];
        
        const glm::vec2 &w1 = uvs[i1];
        const glm::vec2 &w2 = uvs[i2];
        const glm::vec2 &w3 = uvs[i3];
        
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);
        
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }

    tangents.resize(vertices.size());
    bitangents.resize(vertices.size());
    
    for (long a = 0; a < vertices.size(); a++)
    {
        const glm::vec3 &n = normals[a];
        const glm::vec3 &t = tan1[a];
        
        // Gram-Schmidt orthogonalize
        tangents[a] = glm::normalize(t - n * glm::dot(n, t));
        
        // Calculate handedness
        float handedness = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;

        bitangents[a] = handedness * glm::cross(normals[a], tangents[a]);
    }
    

}