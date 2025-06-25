#pragma once

#include <iostream>
#include <ostream>
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

    std::string getSelectedCarObj() {
        auto carFile = std::get<0>(this->carFiles[selectedCar]);
        std::cout << "Selected car: " << carFile << std::endl;

        return carFile;
    }

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

    virtual void keyboardHandler(const unsigned char key) {
        glm::vec3 direction = calculateDirectionVector(useVerticalMovement);
        constexpr float moveSpeed = 1.0f; // Adjust speed as necessary

        switch (key) {
        case 'w': // Move forward
            cameraPos += normalize(direction) * moveSpeed;
            break;
        case 's': // Move backward
            cameraPos -= normalize(direction) * moveSpeed;
            break;
        case 'd': // Strafe right
            cameraPos += normalize(cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
            break;
        case 'a': // Strafe left
            cameraPos -= normalize(cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))) * moveSpeed;
            break;
        case 'q': // Move down
            cameraPos.y -= moveSpeed;
            break;
        case 'e': // Move up
            cameraPos.y += moveSpeed;
            break;
        default:
            break;
        }
    }

    virtual void resetAndInit() {
        cameraPos = startCameraPos();
        cameraAlpha = 0;
        cameraBeta = 0;
    }

    void clearVBO();
    glm::vec3 calculateDirectionVector(bool withVertical);

    glm::vec3 cameraPos = startCameraPos();
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
    float cameraAlpha = 0.0f;
    float cameraBeta = 0.0f;
    bool useVerticalMovement = false;
    UniformVars* uniform_vars;
    skybox* skyboxRef;

    glm::mat4 currentViewMat() {
        glm::vec3 dir = calculateDirectionVector(true);
        glm::vec3 center = cameraPos + dir;

        return lookAt(cameraPos, center, glm::vec3(0.0, 1.0, 0.0));
    }
};
