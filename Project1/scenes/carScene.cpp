#include "carScene.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "../FileSys.h"
#include "../glsl.h"

glm::vec3 carScene::startCameraPos() { return calcCameraPos(); }

glm::vec3 carScene::startCenterPos() { return glm::vec3(0.0f, 0.0f, 0.0f); }

carScene::carScene() { carScene::resetAndInit(); }

constexpr int RADIUS_INCREMENT = 1;
constexpr int HEIGHT_INCREMENT = 1;
constexpr float PERCENTAGE_INCREMENT = 0.02f;

void carScene::keyboardHandler(const unsigned char key) {
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
    case '.':
        objects[selectedCar + 1].visible = false;
        selectedCar = (selectedCar + 1) % carFiles.size();
        objects[selectedCar + 1].visible = true;
        break;
    case ',':
        objects[selectedCar + 1].visible = false;
        selectedCar = (selectedCar - 1) % carFiles.size();
        objects[selectedCar + 1].visible = true;
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
            if (dirContent.find(".obj") != string::npos) { carFiles.push_back(dirContent); }
        }
    }
}

void carScene::resetAndInit() {
    selectedCar = 0;
    camRadius = 6;
    camHeight = 2;
    camPercentage = 0.25;
    cameraPos = startCameraPos();
    centerPos = startCenterPos();
    objects.clear();
    getAllCars();

    addObject("Objects/Eigen/exports/plateau.obj", "textures/Yellobrk.bmp", createMaterial())->
        modelSpace.translate(glm::vec3(0.0, 0.0, 0.0))->scale(0.25f);

    // Add all cars
    for (auto& car : carFiles) { addCar(car.c_str()); }

    objects[selectedCar + 1].visible = true;
}

void carScene::addCar(const char* car_path) {
    addObject(car_path, "textures/uvtemplate_flip.bmp", createMaterial(), false)->
        modelSpace.translate(glm::vec3(0.0, 0.147, 0.0))->scale(0.008f);
}
