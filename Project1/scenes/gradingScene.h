#pragma once
#include "../objectScene.h"

class gradingScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;
    void preRenderCallback(glm::vec3 light_pos) override;
    gradingScene(ApplicationData* application_data);
    gradingScene() = delete;

private:
    void initScene();
    object* getSelectedObj();
    void incrementObjSelection(int num);
    unsigned selectedObjNumber;

    int frameTicker;
};
