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
    void render(UniformVars uVars, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos);
    void debugPrint();
};

class objectScene {
public:
    UniformVars* uniform_vars;
    std::vector<object> objects;
    unsigned int num_objects = 0;

    objectScene();
    object* addObject(const char* obj_path, const char* txt_path, Material* material);
    void render(glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos);
    void setUniformVars(UniformVars* uniform_vars, GLuint program_id);
};
