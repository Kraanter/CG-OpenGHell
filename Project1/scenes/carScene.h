#pragma once
#include "../objectScene.h"

class carScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    carScene();
    void resetAndInit() override;
};
