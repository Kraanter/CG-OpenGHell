#include "gradingScene.h"

gradingScene::gradingScene(ApplicationData* application_data): objectScene(application_data) {
    gradingScene::resetAndInit();
}

glm::vec3 gradingScene::startCameraPos() {
    return glm::vec3(0, 1, 0);
}

void gradingScene::resetAndInit() {
    objectScene::resetAndInit();

    initScene();
}

void gradingScene::initScene() {
    std::cout << "Cheese stengel" << std::endl;

    addGroundPlane(200.0f);
    addObject("Objects/Eigen/exports/plateau.obj", "textures/Yellobrk.bmp", createMaterial());

    // Skybox is handled by objectScene.
}

object* gradingScene::getSelectedObj() {
    return &objects[selectedObjNumber];
}

void gradingScene::incrementObjSelection(int num) {
    if (num < 0) {
        num = static_cast<int>(getNumObjects()) + num;
    }

    frameTicker = 0;

    selectedObjNumber = (selectedObjNumber + num) % getNumObjects();
}

void gradingScene::keyboardHandler(unsigned char key) {
    float change = ((((frameTicker) / 10) / glm::pi<float>()) / 10);
    switch (key) {
    case ' ':
        gradingScene::resetAndInit();
        break;
    case '+':
        getSelectedObj()->modelSpace.translate(glm::vec3(0, 0 - change, 0));
        incrementObjSelection(1);
        break;
    case '-':
        getSelectedObj()->modelSpace.translate(glm::vec3(0, -change, 0));
        incrementObjSelection(-1);
        break;
    case 'i':
        getSelectedObj()->modelSpace.translate(glm::vec3(1, 0, 0));
        break;
    case 'k':
        getSelectedObj()->modelSpace.translate(glm::vec3(-1, 0, 0));
        break;
    case 'u':
        getSelectedObj()->modelSpace.translate(glm::vec3(0, 1, 0));
        break;
    case 'o':
        getSelectedObj()->modelSpace.translate(glm::vec3(0, -1, 0));
        break;
    case 'j':
        getSelectedObj()->modelSpace.translate(glm::vec3(0, 0, 1));
        break;
    case 'l':
        getSelectedObj()->modelSpace.translate(glm::vec3(0, 0, -1));
        break;
    default:
        objectScene::keyboardHandler(key);
        break;
    }
}

void gradingScene::preRenderCallback(glm::vec3 light_pos) {
    objectScene::preRenderCallback(light_pos);

    frameTicker += 1;
    float kokosnoot = (sin(((frameTicker - 1) / 10) / glm::pi<float>()) / 10);
    float kokosnootOld = (sin((frameTicker / 10) / glm::pi<float>()) / 10);
    getSelectedObj()->modelSpace.translate(glm::vec3(0, kokosnootOld - kokosnoot, 0));
}
