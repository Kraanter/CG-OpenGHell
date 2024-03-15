#include <iostream>

#include <GL/glew.h>
#include <vector>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glsl.h"
#include "objloader.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;
GLuint vao;

// Uniforms
GLuint univorm_mvp;

// Matricies
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat4 mvp;

vector<glm::vec3> vertices;
vector<glm::vec2> uvs;
vector<glm::vec3> normals;

// animation
bool animate = false;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
        glutExit();
    if (key == 'a')
        animate = !animate;
    if (key == 'x')
        model = glm::rotate(model, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
    if (key == 'y')
        model = glm::rotate(model, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    if (key == 'z')
        model = glm::rotate(model, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
}

void keyboardSpecialHandler(int key, int a, int b)
{
    if (key == GLUT_KEY_UP)
        model = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
    if (key == GLUT_KEY_DOWN)
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
    if (key == GLUT_KEY_LEFT)
        model = glm::translate(model, glm::vec3(-0.1f, 0.0f, 0.0f));
    if (key == GLUT_KEY_RIGHT)
        model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    // Define background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (animate)
        model = glm::rotate(model, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = projection * view * model;
    
    // Attach to program_id
    glUseProgram(program_id);

    // send mvp
    glUniformMatrix4fv(univorm_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    // Send vao
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    // Swap buffers
    glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(keyboardSpecialHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders()
{
    char* vertexshader = glsl::readFile(vertexshader_name);
    GLuint vsh_id = glsl::makeVertexShader(vertexshader);

    char* fragshader = glsl::readFile(fragshader_name);
    GLuint fsh_id = glsl::makeFragmentShader(fragshader);

    program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers()
{
    GLuint position_id;
    GLuint vbo_vertices;

    // vbo for vertices
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    position_id = glGetAttribLocation(program_id, "position");

    // Allocate vao
    glGenVertexArrays(1, &vao);

    // Bind vao
    glBindVertexArray(vao);

    // Bind vertices to vao
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    univorm_mvp = glGetUniformLocation(program_id, "mvp");
}

void InitMatricies()
{
    model = glm::mat4();

    // model = glm::scale(model, glm::vec3(2.0f, 0.5f, 2.0f));
    // model = glm::translate(model, glm::vec3(2.5f, 0.5f, -10.0f));
    // model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f));

    view = glm::lookAt(
        glm::vec3(0.0f, 2.0f, 4.0f),
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    projection = glm::perspective(
        glm::radians(45.0f),
        (float)WIDTH / (float)HEIGHT,
        0.1f,
        20.0f
    );

    mvp = projection * view * model;
}

void InitObjects()
{
    bool res = loadOBJ("objects/teapot.obj", vertices, uvs, normals);
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitObjects();
    InitShaders();
    InitMatricies();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
