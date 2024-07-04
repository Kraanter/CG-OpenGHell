#include "objectScene.h"

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
    modelSpace.rotate(0.01f, glm::vec3(0.5f, 1.0f, 0.2f));
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

objectScene::objectScene() {}

void objectScene::addObject(const char* obj_path, const char* txt_path, Material* material) {
    objectData data(obj_path, txt_path);
    object obj(data, material);
    objects.push_back(obj);
    num_objects++;
}

void objectScene::bindVBO(GLuint program_id) {
    uniform_vars.uniform_mv = glGetUniformLocation(program_id, "mv");
    uniform_vars.uniform_proj = glGetUniformLocation(program_id, "projection");
    uniform_vars.uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    uniform_vars.uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
    uniform_vars.uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
    uniform_vars.uniform_specular = glGetUniformLocation(program_id, "mat_specular");
    uniform_vars.uniform_material_power = glGetUniformLocation(program_id, "mat_power");

    for (auto& obj : objects) { obj.bindVBO(program_id); }
}

void objectScene::render(glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos) {
    for (auto& obj : objects) { obj.render(uniform_vars, view, projection, light_pos); }
}

void objectScene::fillUniformVars(glm::mat4 projection, glm::vec3 light_pos) {
    glUniformMatrix4fv(uniform_vars.uniform_proj, 1, GL_FALSE, value_ptr(projection));
    glUniform3fv(uniform_vars.uniform_light_pos, 1, value_ptr(light_pos));
}
