#include "renderableObject.h"

#include <iostream>

#include "objloader.h"
#include "texture.h"

renderableObject::renderableObject() = default;

renderableObject::renderableObject(const char* obj_path, const char* txt_path): material(nullptr) {
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
