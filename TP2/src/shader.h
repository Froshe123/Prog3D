#pragma once

#ifndef __APPLE__
#include <GL/gl.h>
#endif

GLuint load_shaders(const char * vertex_file_path,const char * fragment_file_path);
