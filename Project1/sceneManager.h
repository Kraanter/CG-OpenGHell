#pragma once
#include <vector>

#include "objectScene.h"

class sceneManager {
public:
    objectScene* operator[](int index) { return &scenes_[index]; }
    void addScene(objectScene scene) { scenes_.push_back(scene); }
    objectScene* currentScene() { return &scenes_[current_scene_]; }
    void render(glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos);

    void setSelectedScene(unsigned sceneNr) {
        if (sceneNr < scenes_.size())
            current_scene_ = sceneNr;
    }

    void nextScene() {
        current_scene_++;
        if (current_scene_ >= scenes_.size())
            current_scene_ = 0;
    }

    void fillUniformVars(glm::mat4 projection, glm::vec3 light_pos);
    void bindVBO(GLuint program_id);

    sceneManager();
    std::vector<objectScene> scenes_;
    unsigned num_scenes_ = 0;
    unsigned current_scene_ = 0;
    GLuint program_id;
    UniformVars uniform_vars;
};
