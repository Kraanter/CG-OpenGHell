#include "trackScene.h"

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
    cameraPos = startCameraPos();
    centerPos = startCenterPos();
    objects.clear();

    object* ground = addGround();
    ground->modelSpace.scale(GROUND_SIZE);

    object* house = addObject("Objects/Eigen/exports/osso.obj", "textures/colormap_flip.bmp", createMaterial());
    house->modelSpace.translate(glm::vec3(10.0, 0.0, 0.0))->rotate(glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));

    for (unsigned i = 0; i < COUNT; i++) {
        object* obj = addObject("Objects/Eigen/exports/track_curb.obj", "textures/track_curb_texture.bmp",
                                createMaterial());
        obj->modelSpace.translate(glm::vec3(0.0, 0.2, i * 2.0 - COUNT));
    }
}

glm::vec3 trackScene::startCameraPos() { return glm::vec3(-15.0, 10.0, 0.0); }
glm::vec3 trackScene::startCenterPos() { return glm::vec3(0.0, 0.0, 0.0); }

void trackScene::keyboardHandler(unsigned char key) {
    switch (key) {
    case 'i': centerPos.z += 0.1f;
        break;
    case 'k': centerPos.z -= 0.1f;
        break;
    case 'j': centerPos.x += 0.1f;
        break;
    case 'l': centerPos.x -= 0.1f;
        break;
    case 'o': centerPos.y += 0.1f;
        break;
    case 'u': centerPos.y -= 0.1f;
        break;
    default:
        objectScene::keyboardHandler(key);
        break;
    }
}


trackScene::trackScene(ApplicationData* app_data): objectScene(app_data) { trackScene::resetAndInit(); }
