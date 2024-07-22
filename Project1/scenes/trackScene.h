#pragma once
#include "../objectScene.h"

class trackScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    trackScene();
    void resetAndInit() override;
};
