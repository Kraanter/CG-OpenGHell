#include "carScene.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "../glsl.h"

glm::vec3 carScene::startCameraPos() { return calcCameraPos(); }

glm::vec3 carScene::startCenterPos() { return glm::vec3(0.0f, 0.0f, 0.0f); }

carScene::carScene() { carScene::resetAndInit(); }

constexpr int RADIUS_INCREMENT = 1;
constexpr int HEIGHT_INCREMENT = 1;
constexpr float PERCENTAGE_INCREMENT = 0.02;

void carScene::keyboardHandler(unsigned char key) {
    switch (key) {
    case 'w':
        camRadius -= RADIUS_INCREMENT;
        break;
    case 's':
        camRadius += RADIUS_INCREMENT;
        break;
    case 'a':
        camPercentage -= PERCENTAGE_INCREMENT;
        break;
    case 'd':
        camPercentage += PERCENTAGE_INCREMENT;
        break;
    case 'q':
        camHeight -= HEIGHT_INCREMENT;
        break;
    case 'e':
        camHeight += HEIGHT_INCREMENT;
        break;
    default:
        return;
    }
    cameraPos = calcCameraPos();
}

glm::vec3 carScene::calcCameraPos() {
    float angle = camPercentage * 2 * M_PI;
    float x = camRadius * cos(angle);
    float z = camRadius * sin(angle);
    return glm::vec3(x, camHeight, z);
}

void carScene::resetAndInit() {
    camRadius = 6;
    camHeight = 2;
    camPercentage = 0.25;
    cameraPos = startCameraPos();
    centerPos = startCenterPos();
    objects.clear();

    addObject("Objects/Eigen/exports/auto_suv.obj", "textures/auto_texture_flip.bmp", createMaterial())->
        modelSpace.translate(glm::vec3(0.0, 0.147, 0.0));
    addObject("Objects/Eigen/exports/plateau.obj", "textures/Yellobrk.bmp", createMaterial())->
        modelSpace.translate(glm::vec3(0.0, 0.0, 0.0))->scale(glm::vec3(0.25));
}
