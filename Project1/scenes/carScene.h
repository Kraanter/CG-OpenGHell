#pragma once
#include "../objectScene.h"

class carScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    carScene();
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;

private:
    unsigned camRadius;
    unsigned camHeight;
    float camPercentage = 0;
    glm::vec3 calcCameraPos();
};
