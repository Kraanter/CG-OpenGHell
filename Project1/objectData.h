#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class objectData {
public:
    std::vector<glm::mat4> modelSpace;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    GLuint texture_id;
    GLuint vao;

    objectData();
    explicit objectData(const char* obj_path, const char* txt_path);
    void bindVBO(GLuint program_id);
    void setTexture(const char* txt_path);
};
