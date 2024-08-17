#include "carScene.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "../FileSys.h"
#include "../glsl.h"

glm::vec3 carScene::startCameraPos() { return calcCameraPos(); }

glm::vec3 carScene::startCenterPos() { return glm::vec3(0.0f, 0.0f, 0.0f); }

carScene::carScene(ApplicationData* app_data): objectScene(app_data) { carScene::resetAndInit(); }

constexpr int RADIUS_INCREMENT = 1;
constexpr int HEIGHT_INCREMENT = 1;
constexpr float PERCENTAGE_INCREMENT = 0.02f;

void carScene::keyboardHandler(const unsigned char key) {
    switch (key) {
    case 'w':
        if (camRadius - RADIUS_INCREMENT <= 0) { return; }
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
        if (camHeight - HEIGHT_INCREMENT <= 0) { return; }
        camHeight -= HEIGHT_INCREMENT;
        break;
    case 'e':
        camHeight += HEIGHT_INCREMENT;
        break;
    case '.':
        reloadScene();
        break;
    case ',':
        reloadScene();
        break;
    default:
        return;
    }
    cameraPos = calcCameraPos();
}

glm::vec3 carScene::calcCameraPos() {
    float angle = camPercentage * 2.0f * M_PI;
    float x = camRadius * cos(angle);
    float z = camRadius * sin(angle);
    return glm::vec3(x, camHeight, z);
}

void carScene::getAllCars() {
    vector<string> carDirs = FileSys::getFilesInDir("objects/Asseto Corsa");

    // Print all files in the directory
    for (auto& file : carDirs) {
        vector<string> dirContents = FileSys::getFilesInDir(file);
        for (auto& dirContent : dirContents) {
            if (dirContent.find(".obj") != string::npos) { appData->carFiles.push_back(dirContent); }
        }
    }
}

void carScene::resetAndInit() {
    objectScene::resetAndInit();
    appData->selectedCar = 0;
    camRadius = 6;
    camHeight = 2;
    camPercentage = 0.25;
    cameraPos = startCameraPos();
    centerPos = startCenterPos();
    getAllCars();

    initScene();
}

void carScene::initScene() {
    addObject("Objects/Eigen/exports/plateau.obj", "textures/Yellobrk.bmp", createMaterial())->
        modelSpace.translate(glm::vec3(0.0, 0.0, 0.0))->scale(0.25f);

    car = addCar(selectedCar().c_str());
}

void carScene::reloadScene() {
    clearVBO();
    resetAndInit();
    for (auto& obj : objects) { obj.bindVBO(appData->program_id); }
}

object* carScene::addCar(const char* car_path) {
    auto addedCar = addObject(car_path, "textures/uvtemplate.bmp", createMaterial(), true);
    addedCar->modelSpace.translate(glm::vec3(0.0, 0.147, 0.0))->scale(0.008f);
    return addedCar;
}
