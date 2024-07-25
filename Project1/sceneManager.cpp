#include "sceneManager.h"

#include <glm/gtc/type_ptr.hpp>

sceneManager::sceneManager() {
    scenes_ = std::vector<objectScene*>();
    uniform_vars = UniformVars();
};

void sceneManager::render(glm::vec3 light_pos) { scenes_[current_scene_]->render(light_pos); }

void sceneManager::bindVBO(GLuint program_id) {
    this->program_id = program_id;

    uniform_vars.uniform_mv = glGetUniformLocation(program_id, "mv");
    uniform_vars.uniform_proj = glGetUniformLocation(program_id, "projection");
    uniform_vars.uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
    uniform_vars.uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
    uniform_vars.uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
    uniform_vars.uniform_specular = glGetUniformLocation(program_id, "mat_specular");
    uniform_vars.uniform_material_power = glGetUniformLocation(program_id, "mat_power");

    // Set the uniform variables for all scenes
    for (auto& scene : scenes_)
        scene->setUniformVars(&uniform_vars, program_id);
}

void sceneManager::clearVBO() {
    for (auto& scene : scenes_)
        scene->clearVBO();
}

void sceneManager::fillUniformVars(glm::mat4 projection, glm::vec3 light_pos) {
    glUniformMatrix4fv(uniform_vars.uniform_proj, 1, GL_FALSE, value_ptr(projection));
    glUniform3fv(uniform_vars.uniform_light_pos, 1, value_ptr(light_pos));
}
