#pragma once
#include <vector>

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
    void translate(glm::vec3 translation);
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 scale);
    void reset();
};

class object {
public:
    object(objectData data, Material* material);
    objectData data;
    Material* material;
    ModelSpace modelSpace;


    void bindVBO(GLuint program_id);
    void render(GLuint program_id, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos);
};

class objectScene {
public:
    objectScene();

    std::vector<object> objects;
    unsigned int num_objects = 0;

    void addObject(const char* obj_path, const char* txt_path, Material* material);
    void bindVBO(GLuint program_id);
    void render(GLuint program_id, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos);
};
