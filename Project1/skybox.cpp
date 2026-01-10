#include "skybox.h"

#include "objectData.h"
#include "objectScene.h"
#include "texture.h"

#include <array>

skybox::skybox(int size, const char* texture_path) {
    objectRef = createSkyboxData(size, texture_path);
    objectRef->visible = true;
}

object* skybox::createSkyboxData(int size, const char* texture_path) {
    objectData data;
    float s = static_cast<float>(size);

    // Define the vertices of the cube (inside-out for skybox)
    std::vector<glm::vec3> vertices = {
        // Front face
        glm::vec3(-s, -s, s), glm::vec3(s, -s, s), glm::vec3(s, s, s),
        glm::vec3(-s, -s, s), glm::vec3(s, s, s), glm::vec3(-s, s, s),

        // Back face
        glm::vec3(-s, -s, -s), glm::vec3(s, -s, -s), glm::vec3(s, s, -s),
        glm::vec3(-s, -s, -s), glm::vec3(s, s, -s), glm::vec3(-s, s, -s),

        // Right face
        glm::vec3(s, -s, s), glm::vec3(s, -s, -s), glm::vec3(s, s, -s),
        glm::vec3(s, -s, s), glm::vec3(s, s, -s), glm::vec3(s, s, s),

        // Left face
        glm::vec3(-s, -s, -s), glm::vec3(-s, -s, s), glm::vec3(-s, s, s),
        glm::vec3(-s, -s, -s), glm::vec3(-s, s, s), glm::vec3(-s, s, -s),

        // Top face
        glm::vec3(-s, s, s), glm::vec3(s, s, s), glm::vec3(s, s, -s),
        glm::vec3(-s, s, s), glm::vec3(s, s, -s), glm::vec3(-s, s, -s),

        // Bottom face
        glm::vec3(-s, -s, -s), glm::vec3(s, -s, -s), glm::vec3(s, -s, s),
        glm::vec3(-s, -s, -s), glm::vec3(s, -s, s), glm::vec3(-s, -s, s),
    };

    data.vertices = vertices;

    // UVs for a left-sided cross layout (4x3). Order per face matches vertices.
    const float stepU = 1.0f / 4.0f;
    const float stepV = 1.0f / 3.0f;
    auto uvRect = [stepU, stepV](int col, int row) {
        float u0 = col * stepU;
        float v0 = row * stepV;
        float u1 = u0 + stepU;
        float v1 = v0 + stepV;
        return std::array<glm::vec2, 6>{
            glm::vec2(u0, v0), glm::vec2(u1, v0), glm::vec2(u1, v1),
            glm::vec2(u0, v0), glm::vec2(u1, v1), glm::vec2(u0, v1),
        };
    };

    auto uvRectFlipH = [stepU, stepV](int col, int row) {
        float u0 = col * stepU;
        float v0 = row * stepV;
        float u1 = u0 + stepU;
        float v1 = v0 + stepV;
        return std::array<glm::vec2, 6>{
            glm::vec2(u1, v0), glm::vec2(u0, v0), glm::vec2(u0, v1),
            glm::vec2(u1, v0), glm::vec2(u0, v1), glm::vec2(u1, v1),
        };
    };

    // Layout (row 0 at bottom): left, front, right, back on middle row
    auto front = uvRect(1, 1);
    auto back = uvRectFlipH(3, 1);
    auto right = uvRect(2, 1);
    auto left = uvRect(0, 1);
    auto topFace = uvRect(1, 2);
    auto bottomFace = uvRect(1, 0);

    std::vector<glm::vec2> uvs;
    uvs.reserve(vertices.size());
    uvs.insert(uvs.end(), front.begin(), front.end());
    uvs.insert(uvs.end(), back.begin(), back.end());
    uvs.insert(uvs.end(), right.begin(), right.end());
    uvs.insert(uvs.end(), left.begin(), left.end());
    uvs.insert(uvs.end(), topFace.begin(), topFace.end());
    uvs.insert(uvs.end(), bottomFace.begin(), bottomFace.end());
    data.uvs = uvs;

    // Calculate normals for each face
    std::vector<glm::vec3> normals;
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec3 normal = normalize(
            cross(vertices[i + 1] - vertices[i], vertices[i + 2] - vertices[i]));
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
    data.normals = normals;

    if (texture_path != nullptr) {
        data.setTexture(texture_path);
    } else {
        data.texture_id = createSolidTexture(173, 216, 230);
    }

    return new object(data, createMaterial());
}

Material* skybox::createMaterial() {
    auto* material = new Material();

    // Skybox should not be affected by lights
    material->ambient_color = glm::vec3(0.0f);
    material->diffuse_color = glm::vec3(0.0f); // no diffuse lighting
    material->specular_color = glm::vec3(0.0f); // no specular highlights
    material->power = 1.0f;
    material->use_texture_only = true;

    return material;
}
