#pragma once

#include "mesh/mesh.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <common/shader.hpp>

#include "utils/utils.h"

// Ce fichier charge la scène du musée. C'est ici que vous pouvez modifier les textures appliquées

std::vector<Mesh> loadMuseum(){

    std::vector<Mesh> meshes;
    meshes.reserve(32); // évite les realloc pour que les références restent valides

    // chemins vers les ressources
    static const std::string pathMeshes = "./resources/meshes/museum/";
    static const std::string pathShaders = "./src/tp/shaders/";
    static const std::string pathTextures = "./resources/textures/";

    Mesh& ground = meshes.emplace_back(pathMeshes + "ground.obj", pathShaders + "base_vertex.glsl", pathShaders + "ground_fragment.glsl");
    Mesh& walls = meshes.emplace_back(pathMeshes + "walls.obj", pathShaders + "base_vertex.glsl", pathShaders + "walls_fragment.glsl");
    Mesh& terrain = meshes.emplace_back(pathMeshes + "terrain.obj", pathShaders + "base_vertex.glsl", pathShaders + "terrain_fragment.glsl");

    // Même meshs, mais dans différents fichiers par praticité
    Mesh& stand1 = meshes.emplace_back(pathMeshes + "stand_1.obj", pathShaders + "base_vertex.glsl", pathShaders + "stand_fragment.glsl");
    Mesh& stand2 = meshes.emplace_back(pathMeshes + "stand_2.obj", pathShaders + "base_vertex.glsl", pathShaders + "stand_fragment.glsl");
    Mesh& stand3 = meshes.emplace_back(pathMeshes + "stand_3.obj", pathShaders + "base_vertex.glsl", pathShaders + "stand_fragment.glsl");
    Mesh& stand4 = meshes.emplace_back(pathMeshes + "stand_4.obj", pathShaders + "base_vertex.glsl", pathShaders + "stand_fragment.glsl");

    Mesh& painting1 = meshes.emplace_back(pathMeshes + "painting_1.obj", pathShaders + "base_vertex.glsl", pathShaders + "tableau1_fragment.glsl");
    Mesh& painting2 = meshes.emplace_back(pathMeshes + "painting_2.obj", pathShaders + "base_vertex.glsl", pathShaders + "tableau2_fragment.glsl");
    Mesh& painting3 = meshes.emplace_back(pathMeshes + "painting_3.obj", pathShaders + "base_vertex.glsl", pathShaders + "tableau3_fragment.glsl");
    Mesh& painting4 = meshes.emplace_back(pathMeshes + "painting_4.obj", pathShaders + "base_vertex.glsl", pathShaders + "tableau4_fragment.glsl");

    // Textures

    // Question 2.2 Question 2.3.1 Question 2.3.2 Question 3.1
    // (Ajoutez vos textures aux meshes ici.)
        painting4.addTexture("albedoTexture", Texture(pathTextures + "bricks/brick_floor_003_diffuse_4k.jpg"));
        painting4.addTexture("normalTexture", Texture(pathTextures + "bricks/brick_floor_003_nor_gl_4k.jpg"));
        walls.addTexture("albedoTexture", Texture(pathTextures + "planks/Planks012_4K_Color.jpg"));
        ground.addTexture("albedoTexture", Texture(pathTextures + "tiles/Tiles002_4K_Color.jpg"));
        painting2.addTexture("albedoTexture", Texture(pathTextures + "bricks/brick_floor_003_diffuse_4k.jpg"));
        painting3.addTexture("albedoTexture", Texture(pathTextures + "pixelart.png"));
        painting3.textures.at("albedoTexture").setFiltering(GL_NEAREST, GL_NEAREST);
    // Exemple de chargement d'une texture :
    // Texture t = Texture(pathTextures + "uv1.png");

    return meshes;
}