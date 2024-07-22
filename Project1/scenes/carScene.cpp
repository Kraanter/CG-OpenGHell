#include "carScene.h"

glm::vec3 carScene::startCameraPos() { return glm::vec3(0.0, 2.0, 6.0); }

glm::vec3 carScene::startCenterPos() { return glm::vec3(0.0f, 0.0f, 0.0f); }

carScene::carScene() { carScene::resetAndInit(); }

void carScene::resetAndInit() {
    cameraPos = startCameraPos();
    centerPos = startCenterPos();

    addObject("Objects/Eigen/exports/auto_suv.obj", "textures/auto_texture_flip.bmp", createMaterial())->
        modelSpace.translate(glm::vec3(1.0, 0.0, 0.0));
    addObject("Objects/Eigen/exports/auto_suv.obj", "textures/uvtemplate.bmp", createMaterial())->
        modelSpace.translate(glm::vec3(-1.0, 0.0, 0.0));
}
