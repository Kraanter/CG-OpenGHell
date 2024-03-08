#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const unsigned int DELTA_TIME = 10;
const int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

struct VertexFormat
{
    glm::vec4 position;
    glm::vec4 color;
    VertexFormat(glm::vec4 p, glm::vec4 c) : position(p), color(c) {}
};

VertexFormat triangle[] = {
    VertexFormat(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
    VertexFormat(glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
    VertexFormat(glm::vec4(0.0f, 0.5f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
};

//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;
GLuint vbo;
GLuint vao;

float f= 0.5f;
bool up = true;


//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
        glutExit();
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    // Define background
    const glm::vec4 blue = glm::vec4(0.0f, 0.0f, 0.4f, 1.0f);
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(blue));

    // define position of new top corner
    if(up)
    {
        if (f >= 1.0f)
        {
            up = false;
        }
        f += 0.01f;
    }
    else
    {
        if (f <= 0.0f)
        {
            up = true;
        }
        f -= 0.01f;
    }

    // Attach to program_id
    glUseProgram(program_id);

    glm::vec4 v = glm::vec4(0.0, f, 0.0, 1.0);
    glm::vec4 v2 = glm::vec4(0.5, f-0.5, 0.0, 1.0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, // target
     4 * sizeof(glm::vec4), // offset
     sizeof(glm::vec4), // size
     &v); // data

    glBufferSubData(GL_ARRAY_BUFFER, // target
     2 * sizeof(glm::vec4), // offset
     sizeof(glm::vec4), // size
     &v2); // data
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Send vao
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Swap buffers
    glutSwapBuffers();
}

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
    glutTimerFunc(DELTA_TIME, Render, 0);
    
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

void InitBuffers()
{
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint position_id = glGetAttribLocation(program_id, "position");
    GLuint color_id = glGetAttribLocation(program_id, "color");

    // Allocate and assign a Vertex Array Object to our handle
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer(position_id, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);
    glEnableVertexAttribArray(position_id);
    
    glVertexAttribPointer(color_id, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(color_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitShaders();
    InitBuffers();

    // Hide console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    // Main loop
    glutMainLoop();

    return 0;
}
