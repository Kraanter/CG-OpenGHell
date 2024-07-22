#include "objectScene.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ModelSpace::ModelSpace(): model(glm::mat4(1.0f)) {}

void ModelSpace::reset() { model = glm::mat4(1.0f); }
void ModelSpace::translate(glm::vec3 translation) { model = glm::translate(model, translation); }
void ModelSpace::rotate(float angle, glm::vec3 axis) { model = glm::rotate(model, angle, axis); }
void ModelSpace::scale(glm::vec3 scale) { model = glm::scale(model, scale); }


object::object(objectData data, Material* material) : data(data), material(material), modelSpace(ModelSpace()) {}

void object::bindVBO(GLuint program_id) { data.bindVBO(program_id); }

void object::render(UniformVars uVars, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos) {
    // modelSpace.rotate(0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
    glm::mat4 mv = view * modelSpace.model;
    glBindTexture(GL_TEXTURE_2D, data.texture_id);
    glUniform3fv(uVars.uniform_material_ambient, 1, value_ptr(material->ambient_color));
    glUniform3fv(uVars.uniform_material_diffuse, 1, value_ptr(material->diffuse_color));
    glUniform3fv(uVars.uniform_specular, 1, value_ptr(material->specular_color));
    glUniform1f(uVars.uniform_material_power, material->power);
    glUniformMatrix4fv(uVars.uniform_mv, 1, GL_FALSE, value_ptr(mv));

    glBindVertexArray(data.vao);
    glDrawArrays(GL_TRIANGLES, 0, data.vertices.size());
    glBindVertexArray(0);
}

void object::debugPrint() {
    // print this->data.uvs, this->data.normals, this->data.vertices
    std::cout << "UVs:\n";
    std::cout << data.uvs.size() << '\n';
    for (auto& uv : data.uvs) { std::cout << uv.x << ' ' << uv.y << '\n'; }
    // std::cout << "Normals:\n";
    // for (auto& normal : data.normals) { std::cout << normal.x << ' ' << normal.y << ' ' << normal.z << '\n'; }
    // std::cout << "Vertices:\n";
    // for (auto& vertex : data.vertices) { std::cout << vertex.x << ' ' << vertex.y << ' ' << vertex.z << '\n'; }
}

objectScene::objectScene() = default;


object* objectScene::addObject(const char* obj_path, const char* txt_path, Material* material) {
    objectData data(obj_path, txt_path);
    object obj(data, material);
    objects.push_back(obj);
    num_objects++;

    return &objects[num_objects - 1];
}


void objectScene::render(glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos) {
    for (auto& obj : objects)
        if (obj.visible)
            obj.render(*uniform_vars, view, projection, light_pos);
}

void objectScene::setUniformVars(UniformVars* uniform_vars, const GLuint program_id) {
    this->uniform_vars = uniform_vars;

    for (auto& obj : objects)
        obj.bindVBO(program_id);
}
