#pragma once
#include "../objectScene.h"

class trackScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    void keyboardHandler(unsigned char key) override;
    trackScene(ApplicationData* appData);
    trackScene() = delete;
    object* addGround();
    void resetAndInit() override;
    void preRenderCallback(glm::vec3 light_pos) override;

private:
    float upnertia = 0.0f;
    glm::vec3 prevCameraPos;
    glm::vec2 prevCameraAngles;
    bool isFlying = false;
    void updateInertia();
};
