#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objloader.h"
#include "renderableObject.h"
#include "texture.h"

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
glm::mat4* model = new glm::mat4[NUM_OBJECTS];
glm::mat4* mv = new glm::mat4[NUM_OBJECTS];

// Material and light
LightSource light;
Material* material = new Material[NUM_OBJECTS];


//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------

renderableObject* objects = new renderableObject[NUM_OBJECTS];

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
    for (int i = 0; i < NUM_OBJECTS; i++) {
        model[i] = glm::rotate(model[i], 0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
        mv[i] = view * model[i];
        glBindTexture(GL_TEXTURE_2D, objects[i].texture_id);
        glUniform3fv(uniform_material_ambient, 1, value_ptr(material[i].ambient_color));
        glUniform3fv(uniform_material_diffuse, 1, value_ptr(material[i].diffuse_color));
        glUniform3fv(uniform_specular, 1, value_ptr(material[i].specular_color));
        glUniform1f(uniform_material_power, material[i].power);
        glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, value_ptr(mv[i]));

        glBindVertexArray(objects[i].vao);
        glDrawArrays(GL_TRIANGLES, 0, objects[i].vertices.size());
        glBindVertexArray(0);
    }

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
    model[2] = translate(model[2], glm::vec3(0.0, 0.0, 0.0));
    model[1] = translate(model[1], glm::vec3(-2.0, 0.0, 0.0));
    model[0] = translate(model[0], glm::vec3(2.0, 0.0, 0.0));

    view = lookAt(
        glm::vec3(0.0, 2.0, 8.0),
        glm::vec3(0.0, 0.5, 0.0),
        glm::vec3(0.0, 1.0, 0.0));
    projection = glm::perspective(
        glm::radians(45.0f),
        1.0f * WIDTH / HEIGHT, 0.1f,
        20.0f);

    for (int i = 0; i < NUM_OBJECTS; i++)
        mv[i] = view * model[i];
}


//------------------------------------------------------------
// void InitObjects()
//------------------------------------------------------------

void InitObjects() {
    // Objects
    int i = 0;

    objects[i++] = renderableObject("Objects/teapot.obj", "textures/Yellobrk.bmp");
    objects[i++] = renderableObject("Objects/torus.obj", "textures/uvtemplate.bmp");
    objects[i++] = renderableObject("Objects/box.obj", "textures/Shrek.bmp");
}


//------------------------------------------------------------
// void InitMaterialsLight()
//------------------------------------------------------------

void InitMaterialsLight() {
    light.position = glm::vec3(4.0, 4.0, 4.0);
    for (int i = 0; i < NUM_OBJECTS; i++) {
        material[i].ambient_color = glm::vec3(0.2, 0.2, 0.1);
        material[i].diffuse_color = glm::vec3(0.5, 0.5, 0.3);
        material[i].specular_color = glm::vec3(0.5, 0.5, 0.5);
        material[i].power = 50.0;
    }
    // material.diffuse_color = glm::vec3(0.5, 0.0, 0.0);
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers() {
    for (int i = 0; i < NUM_OBJECTS; i++) { objects[i].bindVBO(program_id); }

    // Make uniform vars
    uniform_mv = glGetUniformLocation(program_id, "mv");
    uniform_proj = glGetUniformLocation(program_id, "projection");
    uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
    uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
    uniform_specular = glGetUniformLocation(program_id, "mat_specular");
    uniform_material_power = glGetUniformLocation(program_id, "mat_power");


    // Attach to program (needed to fill uniform vars)
    glUseProgram(program_id);

    // Fill uniform vars
    glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, value_ptr(projection));
    glUniform3fv(uniform_light_pos, 1, value_ptr(light.position));
}


int main(int argc, char** argv) {
    InitGlutGlew(argc, argv);
    InitShaders();
    InitMatrices();
    InitObjects();
    InitMaterialsLight();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
