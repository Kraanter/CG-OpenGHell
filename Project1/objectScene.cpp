#include "objectScene.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"

ModelSpace::ModelSpace(): model(glm::mat4(1.0f)) {}

void ModelSpace::reset() { model = glm::mat4(1.0f); }

ModelSpace* ModelSpace::translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
    return this;
}

ModelSpace* ModelSpace::rotate(float angle, glm::vec3 axis) {
    model = glm::rotate(model, angle, axis);
    return this;
}

ModelSpace* ModelSpace::scale(glm::vec3 scale) {
    model = glm::scale(model, scale);
    return this;
}

ModelSpace* ModelSpace::scale(float scale) {
    model = glm::scale(model, glm::vec3(scale));
    return this;
}

object::object(objectData data, Material* material) : data(data), material(material), modelSpace(ModelSpace()) {}

void object::bindVBO(GLuint program_id) { data.bindVBO(program_id); }

void object::render(const UniformVars* uVars, const glm::mat4* view, glm::vec3 light_pos) {
    glm::mat4 mv = *view * modelSpace.model;
    glBindTexture(GL_TEXTURE_2D, data.texture_id);
    glUniform3fv(uVars->uniform_material_ambient, 1, value_ptr(material->ambient_color));
    glUniform3fv(uVars->uniform_material_diffuse, 1, value_ptr(material->diffuse_color));
    glUniform3fv(uVars->uniform_specular, 1, value_ptr(material->specular_color));
    glUniform1f(uVars->uniform_material_power, material->power);
    glUniformMatrix4fv(uVars->uniform_mv, 1, GL_FALSE, value_ptr(mv));

    glBindVertexArray(data.vao);
    glDrawArrays(GL_TRIANGLES, 0, data.vertices.size());
    glBindVertexArray(0);
}

void object::debugPrint() {
    std::cout << "UVs:\n";
    for (auto& uv : data.uvs) { std::cout << '(' << uv.x << ", " << uv.y << ')'; }
    std::cout << "\nNormals:\n";
    for (auto& normal : data.normals) { std::cout << '(' << normal.x << ", " << normal.y << ", " << normal.z << ')'; }
    std::cout << "\nVertices:\n";
    for (auto& vertex : data.vertices) { std::cout << '(' << vertex.x << ", " << vertex.y << ", " << vertex.z << ')'; }
    std::cout << '\n';
}

void objectScene::clearVBO() {
    for (auto& obj : objects)
        glDeleteVertexArrays(1, &obj.data.vao);
}

objectScene::objectScene(ApplicationData* app_data) : appData(app_data) {
    std::cout << "objectScene constructor\n";
    objectScene::resetAndInit();
    skyboxRef = new skybox(10);
};


object* objectScene::addObject(const char* obj_path, const char* txt_path, Material* material, bool visible) {
    objectData data(obj_path, txt_path);

    return addObject(data, material, visible);
}

object* objectScene::addObject(objectData data, Material* material, bool visible) {
    object obj(data, material);
    obj.visible = visible;
    objects.push_back(obj);
    num_objects++;

    return &objects[num_objects - 1];
}


void objectScene::render(glm::vec3 light_pos) {
    preRenderCallback(light_pos);
    glm::mat4 view = currentViewMat();
    skyboxRef->objectRef->render(uniform_vars, &view, light_pos);
    for (auto& obj : objects)
        if (obj.visible) {
            glm::mat4 curViewMat = currentViewMat();
            obj.render(uniform_vars, &curViewMat, light_pos);
        }
}

void objectScene::setUniformVars(UniformVars* uniform_vars, const GLuint program_id) {
    this->uniform_vars = uniform_vars;
    appData->program_id = program_id;

    for (auto& obj : objects)
        obj.bindVBO(program_id);
}

Material* objectScene::createMaterial() {
    const auto material = new Material();
    material->ambient_color = glm::vec3(0.2, 0.2, 0.1);
    material->diffuse_color = glm::vec3(0.5, 0.5, 0.3);
    material->specular_color = glm::vec3(0.5, 0.5, 0.5);
    material->power = 50.0;
    return material;
}

object* skybox::createSkyboxData(int size) {
    objectData data;
    data.vertices = {
        // Front face
        glm::vec3(-size, -size, -size),
        glm::vec3(-size, -size, size),
        glm::vec3(-size, size, size),
        glm::vec3(-size, -size, -size),
        glm::vec3(-size, size, size),
        glm::vec3(-size, size, -size),
        // Right face
        glm::vec3(size, -size, -size),
        glm::vec3(size, -size, size),
        glm::vec3(size, size, size),
        glm::vec3(size, -size, -size),
        glm::vec3(size, size, size),
        glm::vec3(size, size, -size),
        // Back face
        glm::vec3(-size, -size, -size),
        glm::vec3(-size, size, -size),
        glm::vec3(size, size, -size),
        glm::vec3(-size, -size, -size),
        glm::vec3(size, size, -size),
        glm::vec3(size, -size, -size),
        // Left face
        glm::vec3(-size, -size, size),
        glm::vec3(-size, size, size),
        glm::vec3(size, size, size),
        glm::vec3(-size, -size, size),
        glm::vec3(size, size, size),
        glm::vec3(size, -size, size),
        // Top face
        glm::vec3(-size, size, -size),
        glm::vec3(-size, size, size),
        glm::vec3(size, size, size),
        glm::vec3(-size, size, -size),
        glm::vec3(size, size, size),
        glm::vec3(size, size, -size),
        // Bottom face
        glm::vec3(-size, -size, -size),
        glm::vec3(-size, -size, size),
        glm::vec3(size, -size, size),
        glm::vec3(-size, -size, -size),
        glm::vec3(size, -size, size),
        glm::vec3(size, -size, -size),
    };
    data.uvs = {
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
        glm::vec2(0.55, 0.55),
    };
    // Normals for the skybox are in the opposite direction of the faces
    data.normals = {
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
    };

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
