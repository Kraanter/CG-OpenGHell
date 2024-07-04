#include "objectScene.h"

#include <glm/gtc/matrix_transform.hpp>

ModelSpace::ModelSpace(): model(glm::mat4(1.0f)) {}

void ModelSpace::reset() { model = glm::mat4(1.0f); }
void ModelSpace::translate(glm::vec3 translation) { model = glm::translate(model, translation); }
void ModelSpace::rotate(float angle, glm::vec3 axis) { model = glm::rotate(model, angle, axis); }
void ModelSpace::scale(glm::vec3 scale) { model = glm::scale(model, scale); }


object::object(objectData data, Material* material) : data(data), material(material), modelSpace(ModelSpace()) {}

void object::bindVBO(GLuint program_id) { data.bindVBO(program_id); }

void object::render(GLuint program_id, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos) {}

objectScene::objectScene() {}

void objectScene::addObject(const char* obj_path, const char* txt_path, Material* material) {
    objectData data(obj_path, txt_path);
    object obj(data, material);
    objects.push_back(obj);
    num_objects++;
}

void objectScene::bindVBO(GLuint program_id) { for (auto& obj : objects) { obj.bindVBO(program_id); } }

void objectScene::render(GLuint program_id, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos) {
    for (auto& obj : objects) { obj.render(program_id, view, projection, light_pos); }
}
