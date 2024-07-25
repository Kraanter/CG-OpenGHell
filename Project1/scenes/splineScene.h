#pragma once
#include "../objectScene.h"
#include "../util/CatmullRom.h"

class splineScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;
    // void preRenderCallback(glm::vec3 light_pos) override;
    splineScene();

private:
    void compileTrack();
    void createTrackPart(glm::vec2 p1, glm::vec2 p2);
    CatmullRom trackSpline;
    CatmullRom generateTrackSpline();
};
