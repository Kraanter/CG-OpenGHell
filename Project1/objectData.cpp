﻿#include "objectData.h"

#include <iostream>

#include "objloader.h"
#include "texture.h"

objectData::objectData() = default;

objectData::objectData(const char* obj_path, const char* txt_path) {
    vertices = std::vector<glm::vec3>();
    uvs = std::vector<glm::vec2>();
    normals = std::vector<glm::vec3>();

    bool res;
    res = loadOBJ(obj_path, vertices, uvs, normals);
    if (!res) {
        std::cerr << "Failed to load object file: " << obj_path << '\n';
        exit(1);
    }

    texture_id = loadBMP(txt_path);
}

void objectData::bindVBO(const GLuint program_id) {
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    GLuint normal_buffer;
    glGenBuffers(1, &normal_buffer);
    GLuint uv_buffer;
    glGenBuffers(1, &uv_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint position_id = glGetAttribLocation(program_id, "position");
    GLuint normal_id = glGetAttribLocation(program_id, "normal");
    GLuint uv_id = glGetAttribLocation(program_id, "uv");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Bind vertices to vao
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(position_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(normal_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(uv_id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}