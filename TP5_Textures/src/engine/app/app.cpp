

#include "app.h"


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <thread>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>




App* currentApp; // because methods cant be glfw callbacks

void GLAPIENTRY // GL API ENTRY permet de ne pas avoir de conflit de nom avec les fonctions windows.
    MessageCallback(GLenum source,
                    GLenum type,
                    GLuint id,
                    GLenum severity,
                    GLsizei length,
                    const GLchar* message,
                    const void* userParam )
{
    return ;//
  fprintf( stdout, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void activateGLErrors(){
    if (glDebugMessageCallback == nullptr) return; // OpenGL < 4.3 (macOS)
    glEnable(GL_DEBUG_OUTPUT);        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

inline void showFPS(GLFWwindow* window){

    static const float timeBetweenUpdates = 0.5;
    static double last_time = glfwGetTime();
    static unsigned int nbframes = 0;
    static const std::string windowTitle ("FPS - ");
    double current_time = glfwGetTime();

    nbframes++;
    
    if (current_time - last_time > timeBetweenUpdates){ // seconds
        glfwSetWindowTitle(window, (windowTitle + std::to_string((uint32_t)(nbframes/timeBetweenUpdates))).c_str());
        last_time = current_time;
        nbframes = 0;
    }
}

void App::init()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return ;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( windowWidth, windowHeight, appName.c_str(), NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window." );
        getchar();
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window);

    currentCamera.resize(windowWidth, windowHeight);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return ;
    }


    // During init, enable debug output
    // glDebugMessageCallback n'existe qu'à partir d'OpenGL 4.3 : macOS s'arrête à 4.1,
    // le pointeur vaut alors NULL et l'appeler provoque une segfault.
    if (glDebugMessageCallback != nullptr) {
        glEnable              ( GL_DEBUG_OUTPUT );
        glDebugMessageCallback( MessageCallback, 0 );
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0, 0.0, 0.0, 1.0);


    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_PROGRAM_POINT_SIZE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    glfwSwapInterval(0);

    //glEnable(GL_CULL_FACE); 

}

void App::start(){
    running = true;

    glfwMakeContextCurrent(window);
    while (running && !glfwWindowShouldClose(window)) update();
}

float deltaTime = 0.0f;
double lastFrame = 0.0f;
void App::update(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    

    glfwPollEvents();
    currentCamera.computeMatricesFromInputs();

    glm::mat4 viewProjection = currentCamera.getVP();
    
    for (Mesh & mesh: meshes){
        mesh.render(currentCamera);
    }

    glfwSwapBuffers(window);

    showFPS(window);
}

void App::stop(){
    running = false;
    glfwTerminate();
}