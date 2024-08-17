#pragma once
#include <string>

#include "../objectScene.h"

class carScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    glm::vec3 startCenterPos() override;
    carScene(ApplicationData* appData);
    carScene() = delete;
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;
    unsigned selectedCarId() { return appData->selectedCar; }

private:
    void initScene();
    void reloadScene();
    std::string selectedCar() { return appData->carFiles[appData->selectedCar]; }
    unsigned camRadius;
    unsigned camHeight;
    float camPercentage;
    object* car;
    void getAllCars();
    object* addCar(const char* car_path);
    glm::vec3 calcCameraPos();
};
