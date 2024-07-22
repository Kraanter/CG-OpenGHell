#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objectScene.h"
#include "sceneManager.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

constexpr constexpr int WIDTH = 800, HEIGHT = 800;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

constexpr unsigned int DELTA_TIME = 10;

//--------------------------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------------------------

struct LightSource {
    glm::vec3 position;
};

//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;

// Material and light
LightSource light;


//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------

sceneManager stage_manager;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b) {
    if (key == 27)
        glutExit();

    const int num = key - '0';
    objectScene* scene = stage_manager.currentScene();
    if (num >= 0 && scene->num_objects > num) { scene->objects[num].visible = !scene->objects[num].visible; }

    if (key == 'n') { stage_manager.nextScene(); }
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render() {
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Attach to program_id
    glUseProgram(program_id);
    // Do transformations
    stage_manager.render(light.position);

    // Swap buffers
    glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n) {
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders() {
    char* vertexshader = glsl::readFile(vertexshader_name);
    GLuint vsh_id = glsl::makeVertexShader(vertexshader);

    char* fragshader = glsl::readFile(fragshader_name);
    GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}


//------------------------------------------------------------
// void InitMatrices()
//------------------------------------------------------------

void InitMatrices() {}


//------------------------------------------------------------
// void InitObjects()
//------------------------------------------------------------


Material* createMaterial() {
    const auto material = new Material();
    material->ambient_color = glm::vec3(0.2, 0.2, 0.1);
    material->diffuse_color = glm::vec3(0.5, 0.5, 0.3);
    material->specular_color = glm::vec3(0.5, 0.5, 0.5);
    material->power = 50.0;
    return material;
}

constexpr int COUNT = 1000;

void InitObjects() {
    objectScene scene;
    for (int i = 0; i < COUNT; i++) {
        object* obj = scene.addObject("Objects/Eigen/exports/track_curb.obj", "textures/track_curb_texture.bmp",
                                      createMaterial());
        obj->modelSpace.translate(glm::vec3(0.0, 0.0, i * 2.0 - COUNT));
    }

    scene.cameraPos = glm::vec3(0.0, 12.0, 100.0);

    objectScene scene2;

    scene2.addObject("Objects/Eigen/exports/auto_suv.obj", "textures/auto_texture_flip.bmp", createMaterial())->
           modelSpace.translate(glm::vec3(1.0, 0.0, 0.0));
    scene2.addObject("Objects/Eigen/exports/auto_suv.obj", "textures/uvtemplate.bmp", createMaterial())->
           modelSpace.translate(glm::vec3(-1.0, 0.0, 0.0));

    scene2.cameraPos = glm::vec3(0.0, 2.0, 6.0);

    stage_manager.addScene(scene2);
    stage_manager.addScene(scene);
}


//------------------------------------------------------------
// void InitMaterialsLight()
//------------------------------------------------------------

void InitMaterialsLight() { light.position = glm::vec3(4.0, 4.0, 4.0); }


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------


void InitBuffers() {
    stage_manager.bindVBO(program_id);

    // Attach to program (needed to fill uniform vars)
    glUseProgram(program_id);

    // Fill uniform vars
    stage_manager.fillUniformVars(glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f), light.position);
}


int main(int argc, char** argv) {
    InitGlutGlew(argc, argv);
    InitShaders();
    InitObjects();
    InitMatrices();
    InitMaterialsLight();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
