#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objectScene.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

constexpr constexpr int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

constexpr unsigned int DELTA_TIME = 10;

constexpr unsigned int NUM_OBJECTS = 3;


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
// GLuint* vao = new GLuint[NUM_OBJECTS];
// GLuint* texture_id = new GLuint[NUM_OBJECTS];

// Uniform ID's
GLuint uniform_mv;
GLuint uniform_proj;
GLuint uniform_light_pos;
GLuint uniform_material_ambient;
GLuint uniform_material_diffuse;
GLuint uniform_specular;
GLuint uniform_material_power;

// Matrices
glm::mat4 view, projection;
// glm::mat4* model = new glm::mat4[NUM_OBJECTS];
glm::mat4* mv = new glm::mat4[NUM_OBJECTS];

// Material and light
LightSource light;
// Material* material = new Material[NUM_OBJECTS];


//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------

// objectData* objects = new objectData[NUM_OBJECTS];
objectScene scene;

// vector<glm::vec3>* vertices = new vector<glm::vec3>[NUM_OBJECTS];
// vector<glm::vec3>* normals = new vector<glm::vec3>[NUM_OBJECTS];
// vector<glm::vec2>* uvs = new vector<glm::vec2>[NUM_OBJECTS];


//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b) {
    if (key == 27)
        glutExit();
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
    scene.render(view, projection, light.position);

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

void InitMatrices() {
    view = lookAt(
        glm::vec3(0.0, 2.0, 8.0),
        glm::vec3(0.0, 0.5, 0.0),
        glm::vec3(0.0, 1.0, 0.0));

    projection = glm::perspective(
        glm::radians(45.0f),
        1.0f * WIDTH / HEIGHT, 0.1f,
        20.0f);
}


//------------------------------------------------------------
// void InitObjects()
//------------------------------------------------------------

void InitObjects() {
    scene.addObject("Objects/teapot.obj", "textures/Yellobrk.bmp", new Material());
    scene.addObject("Objects/torus.obj", "textures/uvtemplate.bmp", new Material());
    scene.addObject("Objects/box.obj", "textures/Shrek.bmp", new Material());

    scene.objects[2].modelSpace.translate(glm::vec3(0.0, 0.0, 0.0));
    scene.objects[1].modelSpace.translate(glm::vec3(-2.0, 0.0, 0.0));
    scene.objects[0].modelSpace.translate(glm::vec3(2.0, 0.0, 0.0));
}


//------------------------------------------------------------
// void InitMaterialsLight()
//------------------------------------------------------------

void InitMaterialsLight() {
    light.position = glm::vec3(4.0, 4.0, 4.0);
    for (int i = 0; i < scene.num_objects; i++) {
        scene.objects[i].material->ambient_color = glm::vec3(0.2, 0.2, 0.1);
        scene.objects[i].material->diffuse_color = glm::vec3(0.5, 0.5, 0.3);
        scene.objects[i].material->specular_color = glm::vec3(0.5, 0.5, 0.5);
        scene.objects[i].material->power = 50.0;
    }
    // material.diffuse_color = glm::vec3(0.5, 0.0, 0.0);
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------


void InitBuffers() {
    scene.bindVBO(program_id);

    // Attach to program (needed to fill uniform vars)
    glUseProgram(program_id);

    // Fill uniform vars
    scene.fillUniformVars(projection, light.position);
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
