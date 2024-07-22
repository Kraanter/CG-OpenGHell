#include "trackScene.h"

constexpr unsigned int COUNT = 1000;

void trackScene::resetAndInit() {
    cameraPos = startCameraPos();
    centerPos = startCenterPos();

    for (unsigned i = 0; i < COUNT; i++) {
        object* obj = addObject("Objects/Eigen/exports/track_curb.obj", "textures/track_curb_texture.bmp",
                                createMaterial());
        obj->modelSpace.translate(glm::vec3(0.0, 0.0, i * 2.0 - COUNT));
    }
}

glm::vec3 trackScene::startCameraPos() { return glm::vec3(0.0, 12.0, 100.0); }
glm::vec3 trackScene::startCenterPos() { return glm::vec3(0.0, 0.0, 0.0); }

trackScene::trackScene() { trackScene::resetAndInit(); }
