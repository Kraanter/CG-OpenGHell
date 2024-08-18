#pragma once

#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#include "objectData.h"
#include "skybox.h"

struct UniformVars;

struct ApplicationData {
    unsigned selectedCar;
    std::vector<std::tuple<std::string, std::string>> carFiles;
    GLuint program_id;

    std::string getSelectedCarObj() { return std::get<0>(carFiles[selectedCar]); }
    std::string getSelectedCarTxt() { return std::get<1>(carFiles[selectedCar]); }
};

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

    ModelSpace* setLocation(glm::vec3 location) {
        auto locMat = glm::translate(glm::mat4(1.0f), location);
        model[3] = locMat[3];
        return this;
    }

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

    virtual void keyboardHandler(const unsigned char key) {
        switch (key) {
        case 'w':
            cameraPos.x -= 1.0f;
            centerPos.x -= 1.0f;
            break;
        case 's':
            cameraPos.x += 1.0f;
            centerPos.x += 1.0f;
            break;
        case 'd':
            cameraPos.z -= 1.0f;
            centerPos.z -= 1.0f;
            break;
        case 'a':
            cameraPos.z += 1.0f;
            centerPos.z += 1.0f;
            break;
        case 'q':
            cameraPos.y -= 1.0f;
            centerPos.y -= 1.0f;
            break;
        case 'e':
            cameraPos.y += 1.0f;
            centerPos.y += 1.0f;
            break;
        default: ;
        }
    }

    virtual void resetAndInit() {
        cameraPos = startCameraPos();
        centerPos = startCenterPos();
        objects.clear();
        num_objects = 0;
    }

    void clearVBO();

    glm::vec3 cameraPos = startCameraPos();
    glm::vec3 centerPos = startCenterPos();
    std::vector<object> objects = std::vector<object>();

    objectScene(ApplicationData* appData);
    object* addObject(const char* obj_path, const char* txt_path, Material* material, bool visible = true);
    object* addObject(objectData data, Material* material, bool visible = true);
    virtual void preRenderCallback(glm::vec3 light_pos) {}
    void render(glm::vec3 light_pos);
    void setUniformVars(UniformVars* uniform_vars, GLuint program_id);
    Material* createMaterial();

protected:
    ApplicationData* appData;
    unsigned num_objects = 0;
    UniformVars* uniform_vars;
    skybox* skyboxRef;
    glm::mat4 currentViewMat() { return lookAt(cameraPos, centerPos, glm::vec3(0.0, 1.0, 0.0)); }
};
