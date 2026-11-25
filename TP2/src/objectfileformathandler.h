#pragma once

//Saves or loads an Object File Format (.off) file.
//OFF files are used to store meshes.

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "triangle.h"

bool saveOFF( const std::string& filename ,
              std::vector<glm::vec3>& i_vertices ,
              std::vector<glm::vec3>& i_normals ,
              std::vector<Triangle>& i_triangles,
              bool save_normals = true );


void openOFF( std::string const& filename,
              std::vector<glm::vec3>& o_vertices,
              std::vector<glm::vec3>& o_normals,
              std::vector<Triangle>& o_triangles,
              bool load_normals = true );
