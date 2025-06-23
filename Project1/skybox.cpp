#include "skybox.h"

#include "objectData.h"
#include "objectScene.h"

skybox::skybox(int size) { objectRef = createSkyboxData(size); }

object* skybox::createSkyboxData(int size) {
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

    // Simple UV coordinates
    std::vector<glm::vec2> uvs = {
        // Front face
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

        // Back face
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

        // Right face
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

        // Left face
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

        // Top face
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),

        // Bottom face
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f),
    };
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

    return new object(data, createMaterial());
}

Material* skybox::createMaterial() {
    const auto material = new Material();
    material->ambient_color = glm::vec3(0.2, 0.2, 0.1);
    material->diffuse_color = glm::vec3(0.5, 0.5, 0.3);
    material->specular_color = glm::vec3(0.5, 0.5, 0.5);
    material->power = 50.0;
    return material;
}
