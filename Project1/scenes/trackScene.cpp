#include "trackScene.h"

#include <iostream>
#include <ostream>

constexpr unsigned int COUNT = 2000;
constexpr unsigned int GROUND_SIZE = 4000;

object* trackScene::addGround() {
    auto ground_data = objectData();
    ground_data.setTexture("textures/colormap_flip.bmp");

    ground_data.vertices = {
        glm::vec3(-1.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0, 1.0),
        glm::vec3(-1.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0, 1.0),
        glm::vec3(-1.0, 0.0, 1.0),
    };

    ground_data.uvs = {
        glm::vec2(0.4, 0.2),
        glm::vec2(0.4, 0.2),
        glm::vec2(0.4, 0.2),
        glm::vec2(0.4, 0.2),
        glm::vec2(0.4, 0.2),
        glm::vec2(0.4, 0.2),
    };

    ground_data.normals = {
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0),
    };

    return addObject(ground_data, createMaterial());
}

void trackScene::resetAndInit() {
    objectScene::resetAndInit();
    cameraPos = startCameraPos();
    prevCameraPos = startCameraPos();
    prevCameraPos.y = 10.0f;

    object* ground = addGround();
    ground->modelSpace.scale(GROUND_SIZE);

    for (unsigned i = 0; i < COUNT / 5; i++) {
        object* obj = addObject("Objects/Eigen/exports/puntboom.obj", "textures/colormap_flip.bmp", createMaterial());
        obj->modelSpace.translate(glm::vec3(-6, 0.2, i * 8.0 - COUNT));
    }

    object* house = addObject("Objects/Eigen/exports/osso.obj", "textures/colormap_flip.bmp", createMaterial());
    house->modelSpace.translate(glm::vec3(10.0, 0.0, 0.0))->rotate(glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));

    object* car = addObject(appData->getSelectedCarObj().c_str(), appData->getSelectedCarTxt().c_str(),
                            createMaterial());
    car->modelSpace.translate(glm::vec3(0.0, 0.147, 0.0))->scale(0.013f);

    for (unsigned i = 0; i < COUNT; i++) {
        object* obj = addObject("Objects/Eigen/exports/track_curb.obj", "textures/track_curb_texture.bmp",
                                createMaterial());
        obj->modelSpace.translate(glm::vec3(0.0, 0.2, i * 2.0 - COUNT));
    }
}

glm::vec3 trackScene::startCameraPos() { return glm::vec3(-15.0, 3.0, 10.0); }

void trackScene::updateInertia() {
    if (isFlying) return;

    if (upnertia > 0.0f) { upnertia -= 0.05f; }
    else if (cameraPos.y > 3.0f) { cameraPos.y -= 0.2f; }
    else if (cameraPos.y < 3.0f) { cameraPos.y = 3.0f; }
}


void trackScene::keyboardHandler(unsigned char key) {
    glm::vec3 currentPos;
    glm::vec2 currentAngles;

    switch (key) {
    case 'v':
        // Swap the camera position with the previous camera position
        currentPos = cameraPos;
        cameraPos = prevCameraPos;
        prevCameraPos = currentPos;

        currentAngles = glm::vec2(cameraAlpha, cameraBeta);
        cameraAlpha = prevCameraAngles.x;
        cameraBeta = prevCameraAngles.y;
        prevCameraAngles = currentAngles;

        isFlying = !isFlying;
        break;
    case ' ':
        if (isFlying || cameraPos.y > 3.0f) break;
        upnertia = 1.0f;
        break;
    case 'l':
        cameraAlpha -= 0.1f;
        break;
    case 'j':
        cameraAlpha += 0.1f;
        break;
    case 'i':
        cameraBeta += 0.1f;
        break;
    case 'k':
        cameraBeta -= 0.1f;
        break;
    case 'q':
    case 'e':
        if (!isFlying) break;
    default:
        useVerticalMovement = isFlying;
        objectScene::keyboardHandler(key);
        break;
    }
}

void trackScene::preRenderCallback(glm::vec3 light_pos) { updateInertia(); }

trackScene::trackScene(ApplicationData* app_data): objectScene(app_data) { trackScene::resetAndInit(); }
