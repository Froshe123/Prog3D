#ifndef SHADER_HPP
#define SHADER_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


GLuint loadShadersFromFileGLSL(const char * vertex_file_path,const char * fragment_file_path);

GLuint loadShaders(std::string VertexShaderCode, std::string FragmentShaderCode, std::string vertexShaderName, std::string fragmentShaderName);

#endif
