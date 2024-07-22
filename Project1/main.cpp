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
#include "scenes/carScene.h"
#include "scenes/trackScene.h"

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
    objectScene* scene = stage_manager.currentScene();
    switch (key) {
    case 'w':
        scene->cameraPos.z -= 1.0f;
        scene->centerPos.z -= 1.0f;
        break;
    case 's':
        scene->cameraPos.z += 1.0f;
        scene->centerPos.z += 1.0f;
        break;
    case 'a':
        scene->cameraPos.x -= 1.0f;
        scene->centerPos.x -= 1.0f;
        break;
    case 'd':
        scene->cameraPos.x += 1.0f;
        scene->centerPos.x += 1.0f;
        break;
    case 'q':
        scene->cameraPos.y -= 1.0f;
        scene->centerPos.y -= 1.0f;
        break;
    case 'e':
        scene->cameraPos.y += 1.0f;
        scene->centerPos.y += 1.0f;
        break;
    case 'r':
        scene->centerPos.z -= 1.0f;
        break;
    case 'f':
        scene->centerPos.z += 1.0f;
        break;
    case 't':
        scene->centerPos.x -= 1.0f;
        break;
    case 'g':
        scene->centerPos.x += 1.0f;
        break;
    case 'y':
        scene->centerPos.y -= 1.0f;
        break;
    case 'h':
        scene->centerPos.y += 1.0f;
        break;
    case 27:
        glutExit();
        break;
    case 'n':
        stage_manager.nextScene();
        break;
    case '\\':
        scene->resetAndInit();
        break;
    }

    const int num = key - '0';
    if (num >= 0 && scene->getNumObjects() > num) { scene->objects[num].visible = !scene->objects[num].visible; }
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
// void InitObjects()
//------------------------------------------------------------
void InitScenes() {
    stage_manager.addScene(new trackScene());
    stage_manager.addScene(new carScene());
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
    InitScenes();
    InitMaterialsLight();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
