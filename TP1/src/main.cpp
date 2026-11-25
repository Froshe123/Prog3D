// -------------------------------------------
// based on gMini : a minimal OpenGL/GLUT
// application for 3D graphics.
// Copyright (C) 2006-2026 Tamy Boubekeur,
// Noura Faraj, Nicolas Lutz
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// This code is not entirely clean
// (too many global variables for instance,
// structures in main.cpp), it is
// for learning purposes only.
// -------------------------------------------

#include <GL/glew.h>

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <GL/glut.h>
#include <float.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "triangle.h"
#include "trackballcamera.h"
#include "objectfileformathandler.h"

//Program identifier
GLuint g_programID;

//Global uniform shader values (camera & transforms)
TrackballCamera g_trackballCamera;
float g_scaling;
glm::vec3 g_translation;

//Global uniform locations for transforms
GLuint g_mvpMatrixUniformLocation;
GLuint g_scalingUniformLocation;
GLuint g_translationUniformLocation;

//Global uniform locations for lights
GLuint g_light1OrientationUniformLocation;
GLuint g_light1AmbientUniformLocation;
GLuint g_light1EnabledUniformLocation;

//Global buffer IDs for the triangle to draw
GLuint g_positionBuffer, g_colorBuffer; //For 3.1 and 3.2
GLuint g_elementBuffer; //For 3.2

struct DirectionalPhongLight
{
    DirectionalPhongLight(glm::vec3 orientation, glm::vec3 ambient, bool enabled = true) :
        orientation(orientation),
        ambient(ambient),
        enabled(enabled)
    {}

    glm::vec3 orientation;
    glm::vec3 ambient;
    bool enabled;
};

struct Mesh
{
    //In C++, std::vector is an array of <something> with utility functions like size() or push_back().
    //std::vector builds perfectly contiguous arrays,
    //meaning that triangles[0] is at a distance of n*sizeof(Triangle) bytes from triangles[n].
    std::vector< glm::vec3 > positions;
    std::vector< glm::vec3 > normals;
    std::vector< Triangle > triangles;

    GLuint positionBuffer, normalBuffer, elementbuffer;

    void initTriangleMesh()
    {
        //TODO : make the same triangle as in section 3 by filling positions, normals, and triangles.
        positions.clear();
        normals.clear();
        triangles.clear();

        
        positions.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
        positions.push_back(glm::vec3( 1.0f, -1.0f, 0.0f));
        positions.push_back(glm::vec3( 1.0f,  1.0f, 0.0f));

        normals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

        triangles.push_back(Triangle(0, 1, 2));


    }

    void initBuffers()
    {
        //TODO : initialize the VBOs like in section 3.2.
        //pass positions and normals as array buffers and triangles as an element buffer.
        //Remember to check glBufferData https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml

        
        glGenBuffers(1, &positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * 3 * sizeof(float), positions.data(), GL_STATIC_DRAW);

       
        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);

       
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * 3 * sizeof(unsigned int), triangles.data(), GL_STATIC_DRAW);

        //TODO : configure the vertex attributes like in section 3.

        // position
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // normales
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }
    void clearBuffers()
    {
        //TODO : use glDeleteBuffers https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml
        //and glDisableVertexAttribArray https://registry.khronos.org/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml

        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

      
        glDeleteBuffers(1, &positionBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteBuffers(1, &elementbuffer);
    }

    void draw ()
    {
        //TODO : bind the EBO and draw.


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);


        glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, (void*)0);
    }

    // ------------------------------------
    // Debugging (old OpenGL, do not imitate this)
    // ------------------------------------

    static void debugDrawVector( glm::vec3 const & i_from, glm::vec3 const & i_to )
    {
        glBegin(GL_LINES);
        glVertex3f(i_from[0], i_from[1], i_from[2] );
        glVertex3f(i_to[0], i_to[1], i_to[2] );
        glEnd();
    }


    void debugDrawNormals()
    {
        glLineWidth(1.);
        for(unsigned int pIt = 0 ; pIt < normals.size() ; ++pIt)
        {
            glm::vec3 to = positions[pIt] + normals[pIt]*0.02f;
            debugDrawVector(positions[pIt], to);
        }
    }

};

Mesh g_mesh;

bool g_display_normals;
bool g_display_loaded_mesh;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint g_window;
static unsigned int g_screenwidth = 1600;
static unsigned int g_screenheight = 900;
static bool g_mouseRotatePressed = false;
static bool g_mouseMovePressed = false;
static bool g_mouseZoomPressed = false;
static int g_lastX=0, g_lastY=0, g_lastZoom=0;
static bool g_fullScreen = false;

// ------------------------------------

void fetchUniformLocations()
{
    g_light1OrientationUniformLocation = glGetUniformLocation(g_programID, "u_light1Orientation");
    g_light1AmbientUniformLocation = glGetUniformLocation(g_programID, "u_light1Ambient");
    g_light1EnabledUniformLocation = glGetUniformLocation(g_programID, "u_light1Enabled");

    g_mvpMatrixUniformLocation = glGetUniformLocation(g_programID, "u_mvpMatrix");
}

void updateLightUniforms()
{
    static DirectionalPhongLight light1(glm::vec3(-52.0f,-16.0f,-50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    glUniform3f(g_light1OrientationUniformLocation, light1.orientation.x, light1.orientation.y, light1.orientation.z); //With glm, access vector members like this...
    glUniform3f(g_light1AmbientUniformLocation, light1.ambient[0], light1.ambient[1], light1.ambient[2]); //...or like this
    //You can also pass a glm vector directly like this:
    //glUniform3fv(g_light1AmbientUniformLocation, 1, glm::value_ptr(light1.ambient)); //glm::value_ptr decays a glm object to its base representation (float* here).
    glUniform1i(g_light1EnabledUniformLocation, light1.enabled); //Check the Khronos documentation on GlUniform https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml
}

void updateTransformUniforms()
{
    glm::mat4 mvpMatrix = g_trackballCamera.perspective() * g_trackballCamera.view();

    glUniformMatrix4fv(g_mvpMatrixUniformLocation, 1, false, glm::value_ptr(mvpMatrix));
}

void init()
{
    static const GLfloat positionBufferData[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };

    static const GLfloat colorBufferData[] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    Triangle elementBufferData(0, 1, 2);

    g_trackballCamera.resize (g_screenwidth, g_screenheight);
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);

    g_display_normals = false;
    g_display_loaded_mesh = true;

    g_scaling = 1.;
    g_translation = glm::vec3(0., 0., 0.);
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    //TODO (3.1) : use glGenBuffers, glBindBuffer, and glBufferData to send
    // the array buffers of positions (positionBufferData) and colors (colorBufferData) to VBOs on the GPU.
    //glGenBuffers creates x buffer objects in OpenGL and writes their ID in a variable or an array of variables.
    //glBindBuffer associates a buffer object specified with its ID to a given target in the GPU.
    //glBufferData sends data to the buffer associated to the given target in the GPU.
    //Hint: glBufferData can use sizeof(something) for the second argument.
    // position 
    glGenBuffers(1, &g_positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positionBufferData), positionBufferData, GL_STATIC_DRAW);

    // couleurs
    glGenBuffers(1, &g_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, g_colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);




    //TODO (3.2) : use glGenBuffers, glBindBuffer, and glBufferData to send
    // the element array buffer of indices (g_elementBuffer) to the GPU.
    // These elements are defined in the triangle elementBufferData.
    // Hint: The element array is a member of the Triangle class. How can you send its adress as float*?
    glGenBuffers(1, &g_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementBufferData), elementBufferData.v, GL_STATIC_DRAW);




    //TODO (3.1): use glEnableVertexAttribArray, glBindBuffer, and glVertexAttribPointer to specify how data previously given to the GPU are read.
    // Hint: There are two vertex attribute arrays to draw, one for positions and the other for colors.
    // Hint: The position buffer will take attribute 0 and the color will take attribute 1. This cannot be changed without tweaking the shader.
    // Hint: Each vertex is defined by three floats for positions and free floats for colors. One triangle includes 3 vertices.

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, g_positionBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, g_colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


}

// ------------------------------------
// GLUT function implementations
// GLUT is an OpenGL helper which allows to create window systems for OpenGL applications,
// implementing both display and input management.
// GLUT requires several functions to be given as function pointers to determine its behaviour,
// such as how to draw, what to do when nothing happens, how to handle keys or mouse movement.
// ------------------------------------

void idle ()
{
    // glutPostRedisplay orders a display of the scene : https://www.opengl.org/resources/libraries/glut/spec3/node20.html
    // In theory we don't always need to redraw the scene every frame.
    glutPostRedisplay ();
}

void display ()
{
    //Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shaders
    glUseProgram(g_programID);

    // Update all uniforms from the last frame
    updateTransformUniforms();
    updateLightUniforms();

    // TODO : uncomment the following lines when mesh is supposedly implemented
     g_mesh.draw();
     if(g_display_normals)
     {
         glColor3f(1.,0.,0.);
         g_mesh.debugDrawNormals();
     }

    // TODO (3.1) : Draw the triangle using glDrawArrays: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml
    //glDrawArrays(GL_TRIANGLES, 0, 3);



    // TODO (3.2) : Draw the triangle using glDrawElements: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
    // You must first bind the element buffer to the target GL_ELEMENT_ARRAY_BUFFER before drawing.
    // Do not use both glDrawArrays and glDrawElements or you will draw the triangle twice.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_elementBuffer);
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);



    // Since we are using double buffering we need to call this.
    glutSwapBuffers();
}

void keyboard (unsigned char keyPressed, int x, int y)
{
    switch (keyPressed)
    {
    case 'f':
        if (g_fullScreen == true) {
            glutReshapeWindow (g_screenwidth, g_screenheight);
            g_fullScreen = false;
        } else {
            glutFullScreen ();
            g_fullScreen = true;
        }
        break;

    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(polygonMode[0] != GL_FILL)
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 'n': //Press n key to display normals
        g_display_normals = !g_display_normals;
        break;

    case '+': //Press + key to increase scale
        g_scaling += 0.005;
        break;

    case '-': //Press - key to decrease scale
        g_scaling -= 0.005;
        break;

    case 'd': //Press d key to translate on x positive
        g_translation[0] += 0.005;
        break;

    case 'q': //Press q key to translate on q negative
        g_translation[0] -= 0.005;
        break;

    case 'z': //Press z key to translate on z positive
        g_translation[1] += 0.005;
        break;

    case 's': //Press s key to translate on y negative
        g_translation[1] -= 0.005;
        break;

    case '1': //Toggle loaded mesh display
        g_display_loaded_mesh = !g_display_loaded_mesh;
        break;

    default:
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        g_mouseMovePressed = false;
        g_mouseRotatePressed = false;
        g_mouseZoomPressed = false;
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            g_trackballCamera.beginRotate (x, y);
            g_mouseMovePressed = false;
            g_mouseRotatePressed = true;
            g_mouseZoomPressed = false;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            g_lastX = x;
            g_lastY = y;
            g_mouseMovePressed = true;
            g_mouseRotatePressed = false;
            g_mouseZoomPressed = false;
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            if (g_mouseZoomPressed == false)
            {
                g_lastZoom = y;
                g_mouseMovePressed = false;
                g_mouseRotatePressed = false;
                g_mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y)
{
    if (g_mouseRotatePressed == true)
    {
        g_trackballCamera.rotate(x, y);
    }
    else if (g_mouseMovePressed == true)
    {
        g_trackballCamera.move(
            (x-g_lastX)/static_cast<float>(g_screenwidth),
            (g_lastY-y)/static_cast<float>(g_screenheight)
        );
        g_lastX = x;
        g_lastY = y;
    }
    else if (g_mouseZoomPressed == true) {
        g_trackballCamera.zoom (float (y-g_lastZoom)/g_screenheight);
        g_lastZoom = y;
    }
}

void reshape (int w, int h)
{
    g_trackballCamera.resize (w, h);
}

// ------------------------------------

int main(int argc, char ** argv)
{
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (g_screenwidth, g_screenheight);
    g_window = glutCreateWindow ("TP HAI719I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (keyboard);
    glutMouseFunc (mouse);
    glutMotionFunc (motion);
    glutReshapeFunc (reshape);

    keyboard ('?', 0, 0);

    //Look into data to find other meshes
    openOFF("data/elephant_n.off", g_mesh.positions, g_mesh.normals, g_mesh.triangles);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Create and compile our GLSL program from the shaders
    g_programID = load_shaders( "vertex_shader.glsl", "fragment_shader.glsl" );
    fetchUniformLocations();
    updateLightUniforms();
    updateTransformUniforms();

    //Use one of these
    g_mesh.initTriangleMesh();
    g_mesh.initBuffers();



    glutMainLoop ();

    // Cleanup VBO and shader
    g_mesh.clearBuffers();
    glDeleteProgram(g_programID);

    return EXIT_SUCCESS;
}

