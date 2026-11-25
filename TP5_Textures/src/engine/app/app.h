

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <thread>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "utils/utils.h"
#include "mesh/mesh.h"
#include "camera/camera.h"

class App {

friend void handleWindowResizedCallback(GLFWwindow* window, int width, int height);
protected:

    void render();

    GLFWwindow* window;
    bool running = false;

    void handleWindowResized();

    void init();

    const int windowWidth = 1280;
    const int windowHeight = 720;
    const std::string appName = "TP Textures";

public:

    std::vector< Mesh > meshes;

    Camera currentCamera;

    App(){
        init();
    }

    inline bool isRunning() const { return running;}

    GLFWwindow* getWindow() {
        return window;
    }

    void update();

    void start();
    void stop();
};