#pragma once
#include "../objectScene.h"
#include "../util/CatmullRom.h"

class splineScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;
    // void preRenderCallback(glm::vec3 light_pos) override;
    splineScene();

private:
    CatmullRom trackSpline;
    CatmullRom generateTrackSpline();
};
