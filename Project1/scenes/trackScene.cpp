#include "trackScene.h"

constexpr unsigned int COUNT = 2000;

void trackScene::resetAndInit() {
    cameraPos = startCameraPos();
    centerPos = startCenterPos();
    objects.clear();

    for (unsigned i = 0; i < COUNT; i++) {
        object* obj = addObject("Objects/Eigen/exports/track_curb.obj", "textures/track_curb_texture.bmp",
                                createMaterial());
        obj->modelSpace.translate(glm::vec3(0.0, 0.0, i * 2.0 - COUNT));
    }
}

glm::vec3 trackScene::startCameraPos() { return glm::vec3(0.0, 10.0, 50.0); }
glm::vec3 trackScene::startCenterPos() { return glm::vec3(0.0, 0.0, 0.0); }

trackScene::trackScene(ApplicationData* app_data): objectScene(app_data) { trackScene::resetAndInit(); }
