#pragma once
#include "../objectScene.h"

class trackScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    void keyboardHandler(unsigned char key) override;
    trackScene(ApplicationData* appData);
    trackScene() = delete;
    object* addGround();
    void resetAndInit() override;
};
