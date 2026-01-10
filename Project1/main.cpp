#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileSys.h"
#include "glsl.h"
#include "objectScene.h"
#include "sceneManager.h"
#include "scenes/carScene.h"
#include "scenes/splineScene.h"
#include "scenes/trackScene.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

constexpr int WIDTH = 800, HEIGHT = 800;

auto fragshader_name = "fragmentshader.frag";
auto vertexshader_name = "vertexshader.vert";

const auto startProjection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 5000.0f);

constexpr unsigned int DELTA_TIME = 10;

auto app_data = ApplicationData{0, vector<tuple<string, string>>(), 0};

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
bool isPaused = false;
int windowWidth = WIDTH;
int windowHeight = HEIGHT;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b) {
    objectScene* scene = stage_manager.currentScene();
    switch (key) {
    case 27:
        isPaused = !isPaused;
        glutSetCursor(isPaused ? GLUT_CURSOR_LEFT_ARROW : GLUT_CURSOR_NONE);
        glutSetWindowTitle(isPaused ? "Hello OpenGL - Paused" : "Hello OpenGL");
        if (!isPaused) {
            glutWarpPointer(windowWidth / 2, windowHeight / 2);
        }
        break;
    case 'n':
        stage_manager.nextScene();
        break;
    case 'r':
        stage_manager.clearVBO();
        scene->resetAndInit();
        stage_manager.bindVBO(program_id);
        break;
    case 'c':
        app_data.nextCarColor();
        stage_manager.onCarColorChanged();
        break;
    default:
        if (scene != nullptr) { scene->keyboardHandler(key); }
        break;
    }
}

//--------------------------------------------------------------------------------
// Mouse handling
//--------------------------------------------------------------------------------

void mouseMoveHandler(int x, int y) {
    static bool warping = false;
    if (isPaused) { return; }
    if (warping) {
        warping = false;
        return;
    }

    int centerX = windowWidth / 2;
    int centerY = windowHeight / 2;
    int dx = x - centerX;
    int dy = y - centerY;

    const float sensitivity = 0.005f;
    objectScene* scene = stage_manager.currentScene();
    if (scene != nullptr) {
        scene->rotateCamera(-static_cast<float>(dx) * sensitivity, -static_cast<float>(dy) * sensitivity);
    }

    warping = true;
    glutWarpPointer(centerX, centerY);
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render() {
    static bool first = true;
    if (first) {
        std::cout << "[render] first frame" << std::endl;
        first = false;
    }
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Attach to program_id
    glUseProgram(program_id);
    // Do transformations
    stage_manager.render(light.position, isPaused);

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
    glutReshapeFunc([](int w, int h) {
        windowWidth = w;
        windowHeight = h;
        glViewport(0, 0, w, h);

        stage_manager.fillUniformVars(
            glm::perspective(glm::radians(45.0f), static_cast<float>(w) / static_cast<float>(h), 0.1f, 5000.0f),
            light.position);
    });
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutPassiveMotionFunc(mouseMoveHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glewInit();

    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(windowWidth / 2, windowHeight / 2);
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
void InitScenes(ApplicationData* app_data) {
    stage_manager.addScene(new trackScene(app_data));
    stage_manager.addScene(new carScene(app_data));
    stage_manager.addScene(new splineScene(app_data));
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
    stage_manager.fillUniformVars(startProjection, light.position);
}


void getAllCars(ApplicationData* appData) {
    if (appData->carFiles.size() > 0) { return; }

    vector<string> carDirs = FileSys::getFilesInDir("objects/Asseto Corsa");

    // Print all files in the directory
    for (auto& file : carDirs) {
        vector<string> dirContents = FileSys::getFilesInDir(file);
        string objFile = "";
        string textureFile = "";
        for (auto& dirContent : dirContents) {
            if (dirContent.find(".obj") != string::npos) { objFile = dirContent; }
            else if (dirContent.find(".bmp") != string::npos) { textureFile = dirContent; }
        }
        std::cout << "Car: " << objFile << " Texture: " << textureFile << endl;
        appData->carFiles.push_back({objFile, textureFile});
    }
}

int main(int argc, char** argv) {
    srand(time(nullptr));
    std::cout << "[init] InitGlutGlew" << std::endl;
    InitGlutGlew(argc, argv);
    std::cout << "[init] InitShaders" << std::endl;
    InitShaders();
    std::cout << "[init] getAllCars" << std::endl;
    getAllCars(&app_data);
    std::cout << "[init] InitScenes" << std::endl;
    InitScenes(&app_data);
    std::cout << "[init] InitMaterialsLight" << std::endl;
    InitMaterialsLight();
    std::cout << "[init] InitBuffers" << std::endl;
    InitBuffers();
    std::cout << "[init] entering main loop" << std::endl;

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
