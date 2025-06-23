#pragma once
#include <string>

#include "../objectScene.h"

class carScene : public objectScene {
public:
    glm::vec3 startCameraPos() override;
    carScene(ApplicationData* appData);
    carScene() = delete;
    void keyboardHandler(unsigned char key) override;
    void resetAndInit() override;
    unsigned selectedCarId() { return appData->selectedCar; }

private:
    void initScene();
    void reloadScene();
    std::string selectedCarObj() { return appData->getSelectedCarObj(); }
    std::string selectedCarTxt() { return appData->getSelectedCarTxt(); }
    unsigned camRadius;
    unsigned camHeight;
    float camPercentage;
    void nextCar();
    void prevCar();
    void reloadCar();
    object* car;
    void getAllCars();
    object* addCar(const char* car_path, const char* texture_path);
    glm::vec3 calcCameraPos();
};
