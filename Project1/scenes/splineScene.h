#pragma once
#include "../glsl.h"
#include "../objectScene.h"
#include "../util/CatmullRom.h"

class splineScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;
    void preRenderCallback(glm::vec3 light_pos) override;
    void onSelectedCarChanged() override;
    void onCarColorChanged() override;
    splineScene(ApplicationData* appData);
    splineScene() = delete;

private:
    void compileTrack();
    void createTrackPart(glm::vec2 p1, glm::vec2 p2);
    float trackCompletion = 0.0f;
    object* car;
    void updateTrackCompletion();
    bool lockedCamera = false;
    bool inCarView = false;
    glm::vec3 carPos = glm::vec3(0.0f);
    float carYaw = 0.0f;
    glm::vec3 currentTrackPos();
    glm::vec2 currentTrackDir();
    CatmullRom trackSpline;
    CatmullRom generateTrackSpline();
};
