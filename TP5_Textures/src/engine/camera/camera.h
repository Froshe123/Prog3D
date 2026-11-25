#pragma once
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{

protected:
    glm::mat4 projection;
    bool rotating = false;
public:
    glm::mat4 transform; // !! is actually the inverse view matrix

    Camera(){
        transform = glm::translate(glm::rotate(glm::mat4(1.0f), (float)M_PI, glm::vec3(0, 1, 0)), glm::vec3(0, 0.5, 0));


        

        projection = glm::perspective(
            glm::radians(60.0f),
            1.0f,
            0.1f,
            300.0f
        );
    }

    void resize(int u, int v){

        projection = glm::perspective(
            glm::radians(60.0f),
            (float)u / (float)v,
            0.1f,
            300.0f
        );

    }

    inline glm::mat4 getProjectionMatrix() const{
        return projection;
    }

    inline glm::mat4 getViewMatrix() const{
        return glm::inverse(transform);
    }

    inline glm::vec3 getGlobalPosition(){
        return glm::vec3(transform[3]); 
    }

    inline glm::mat4 getVP() const{
        return getProjectionMatrix() * getViewMatrix();
    }

    inline glm::vec3 getForwardVector() const{
        return -glm::vec3(transform[2][0], transform[2][1], transform[2][2]);
    }
    inline glm::vec3 getUpVector() const{
        return glm::vec3(transform[1][0], transform[1][1], transform[1][2]);
    }
    inline glm::vec3 getRightVector() const{
        return glm::vec3(transform[0][0], transform[0][1], transform[0][2]);
    }

    float mouseSpeed = 4.0;
    float speed = 15.0;

    void computeMatricesFromInputs(){
        if (!glfwGetCurrentContext()) return;

        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // Get mouse position
        static double xpos_last, ypos_last;
        double xpos, ypos;
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);


        // Compute new orientation

        if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){

            float horizontalAngle = mouseSpeed * (xpos - xpos_last) / 1000.0;
            float verticalAngle   = mouseSpeed * (ypos - ypos_last) / 1000.0;

            // in local space
            transform = glm::rotate(transform, -verticalAngle, glm::vec3(1.0, 0.0, 0.0));

            // in global space (but without the translate)
            auto horizontalRotationMatrix = glm::mat4(1.0);
            horizontalRotationMatrix = glm::rotate(horizontalRotationMatrix, -horizontalAngle, glm::vec3(0.0, 1.0, 0.0));
            
            auto translation = transform[3];
            transform[3] = glm::vec4(0.0, 0.0, 0.0, 1.0);

            transform = horizontalRotationMatrix * transform;
            transform[3] = translation;
        }
        lastTime = currentTime;

        xpos_last = xpos;
        ypos_last = ypos;

        // Move forward

        if (
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_UP ) == GLFW_PRESS ||
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_W ) == GLFW_PRESS
        ){
            transform = glm::translate(
                transform,
                glm::vec3(0.0, 0.0, -1.0) * deltaTime * speed
            );
        }
        // Move backward
        if (
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_DOWN ) == GLFW_PRESS ||
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_S ) == GLFW_PRESS
        ){
            transform = glm::translate(
                transform,
                glm::vec3(0.0, 0.0, 1.0) * deltaTime * speed
            );
        }
        // Strafe right
        if (
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_RIGHT ) == GLFW_PRESS ||
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_D ) == GLFW_PRESS
        ){
            transform = glm::translate(
                transform,
                glm::vec3(1.0, 0.0, 0.0) * deltaTime * speed
            );
        }
        // Strafe left
        if (
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_LEFT ) == GLFW_PRESS ||
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_A ) == GLFW_PRESS
        ){
            transform = glm::translate(
                transform,
                glm::vec3(-1.0, 0.0, 0.0) * deltaTime * speed
            );
        }
        // Go Up
        if (glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_SPACE ) == GLFW_PRESS){
            transform[3] += (
                glm::vec4(0.0, 1.0, 0.0, 0.0) * deltaTime * speed
            );
        }
        // Go Down
        if (
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS ||
            glfwGetKey( glfwGetCurrentContext(), GLFW_KEY_RIGHT_SHIFT ) == GLFW_PRESS
        ){
           transform[3] += (
                glm::vec4(0.0, -1.0, 0.0, 0.0) * deltaTime * speed
            );
        }
	}
};