#pragma once
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "objectData.h"

struct Material {
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float power;
};

class ModelSpace {
public:
    glm::mat4 model;

    ModelSpace();
    ModelSpace* translate(glm::vec3 translation);
    ModelSpace* rotate(float angle, glm::vec3 axis);
    ModelSpace* scale(glm::vec3 scale);
    ModelSpace* scale(float scale);
    void reset();
};

struct UniformVars {
    GLuint uniform_mv;
    GLuint uniform_proj;
    GLuint uniform_light_pos;
    GLuint uniform_material_ambient;
    GLuint uniform_material_diffuse;
    GLuint uniform_specular;
    GLuint uniform_material_power;
};

class object {
public:
    object(objectData data, Material* material);
    objectData data;
    Material* material;
    ModelSpace modelSpace;
    bool visible = true;


    void bindVBO(GLuint program_id);
    void render(const UniformVars* uVars, const glm::mat4* view, glm::vec3 light_pos);
    void debugPrint();
};

class objectScene {
public:
    unsigned getNumObjects() { return num_objects; }

    virtual glm::vec3 startCameraPos() { return glm::vec3(1.0, 1.0, 0.0); }
    virtual glm::vec3 startCenterPos() { return glm::vec3(0.0, 0.0, 0.0); }

    virtual void keyboardHandler(unsigned char key) {
        switch (key) {
        case 'w':
            cameraPos.z -= 0.1;
            centerPos.z -= 0.1;
            break;
        case 's':
            cameraPos.z += 0.1;
            centerPos.z += 0.1;
            break;
        case 'a':
            cameraPos.x -= 0.1;
            centerPos.x -= 0.1;
            break;
        case 'd':
            cameraPos.x += 0.1;
            centerPos.x += 0.1;
            break;
        case 'q':
            cameraPos.y -= 0.1;
            centerPos.y -= 0.1;
            break;
        case 'e':
            cameraPos.y += 0.1;
            centerPos.y += 0.1;
            break;
        }
    }

    virtual void resetAndInit() {
        cameraPos = startCameraPos();
        centerPos = startCenterPos();
        objects.clear();
    }

    glm::vec3 cameraPos = startCameraPos();
    glm::vec3 centerPos = startCenterPos();
    std::vector<object> objects = std::vector<object>();

    objectScene();
    object* addObject(const char* obj_path, const char* txt_path, Material* material);
    void render(glm::vec3 light_pos);
    void setUniformVars(UniformVars* uniform_vars, GLuint program_id);
    Material* createMaterial();

private:
    unsigned num_objects = 0;
    UniformVars* uniform_vars;
    glm::mat4 currentViewMat() { return lookAt(cameraPos, centerPos, glm::vec3(0.0, 1.0, 0.0)); }
};
