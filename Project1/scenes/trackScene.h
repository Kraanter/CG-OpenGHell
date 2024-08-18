#pragma once
#include "../objectScene.h"

class trackScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    void keyboardHandler(unsigned char key) override;
    trackScene(ApplicationData* appData);
    glm::vec3 calcCenterPos();
    trackScene() = delete;
    object* addGround();
    void resetAndInit() override;
    void preRenderCallback(glm::vec3 light_pos) override;

private:
    float upnertia = 0.0f;
    float cameraRotationHor = 0.0f;
    float cameraRotationVer = 0.0f;
    void updateInertia();
};
