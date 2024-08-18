#include "splineScene.h"

#include <iostream>

splineScene::splineScene(ApplicationData* application_data): objectScene(application_data) {
    splineScene::resetAndInit();
}

glm::vec3 splineScene::startCameraPos() { return glm::vec3(0.01, 1, 0); }
glm::vec3 splineScene::startCenterPos() { return glm::vec3(0, 0, 0); }

void splineScene::keyboardHandler(unsigned char key) {
    switch (key) {
    case 'e':
        cameraPos.y += 0.1;
        break;
    case 'q':
        cameraPos.y -= 0.1;
        break;
    default:
        objectScene::keyboardHandler(key);
        break;
    }
}

glm::vec3 splineScene::currentTrackPos() {
    auto p = trackSpline.getPoint(trackCompletion);
    return glm::vec3(p.x, 0, p.y);
}

glm::vec2 splineScene::currentTrackDir() {
    auto p = trackSpline.getDerivative(trackCompletion);
    return glm::vec2(p.x, p.y);
}

void splineScene::updateTrackCompletion() {
    trackCompletion += 0.001f;
    if (trackCompletion > 1.0f) { trackCompletion = 0.0f; }
}

void splineScene::preRenderCallback(glm::vec3 light_pos) {
    updateTrackCompletion();

    centerPos = currentTrackPos();

    if (car != nullptr) {
        auto dir = currentTrackPos();

        car->modelSpace.setLocation(dir);
    }
}

void splineScene::resetAndInit() {
    objectScene::resetAndInit();
    cameraPos = startCameraPos();
    centerPos = startCenterPos();

    trackSpline = generateTrackSpline();
    compileTrack();

    car = addObject(appData->getSelectedCarObj().c_str(), "textures/uvtemplate.bmp", createMaterial(), true);
    car->modelSpace.scale(0.0001f);
}

#define TRACKWIDTH 0.03f

void splineScene::compileTrack() {
    // Create a track from the spline
    constexpr int numPoints = 5000;
    auto first = trackSpline.getPoint(1.0f / numPoints);
    glm::vec2 last = first;

    for (float i = 0; i < numPoints - 1; i += 1.0f) {
        float t1 = i / (numPoints - 1.0f);
        glm::vec2 p1 = trackSpline.getPoint(t1);
        createTrackPart(p1, last);
        last = p1;
    }

    createTrackPart(last, first);
}

void splineScene::createTrackPart(glm::vec2 p1, glm::vec2 p2) {
    glm::vec2 dir = normalize(p2 - p1);
    float angle = glm::atan(dir.y, dir.x);
    float length = glm::length(p2 - p1);

    auto dir3d = glm::vec3(dir.x, dir.y, 0.0f);
    auto crossDir3d = normalize(cross(dir3d, glm::vec3(0.0f, 0.0f, 1.0f)));
    auto crossDir = glm::vec2(crossDir3d.x, crossDir3d.y);

    auto p3 = p1 + crossDir * TRACKWIDTH;
    auto p4 = p2 + crossDir * TRACKWIDTH;
    auto p5 = p2 - crossDir * TRACKWIDTH;
    auto p6 = p1 - crossDir * TRACKWIDTH;

    auto data = objectData();
    data.setTexture("textures/track.bmp");
    data.vertices = {
        // 6 points to create 2 triangles to create a quad p3-p4-p5-p6
        glm::vec3(p3.x, 0.0f, p3.y),
        glm::vec3(p4.x, 0.0f, p4.y),
        glm::vec3(p5.x, 0.0f, p5.y),
        glm::vec3(p3.x, 0.0f, p3.y),
        glm::vec3(p5.x, 0.0f, p5.y),
        glm::vec3(p6.x, 0.0f, p6.y),

    };
    data.uvs = {
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
    };
    data.normals = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    auto obj = object(data, new Material());
    objects.push_back(obj);
    num_objects++;
}

CatmullRom splineScene::generateTrackSpline() {
    auto spline = CatmullRom();
    // Desmos: https://www.desmos.com/calculator/qdk6xeflti
    // This might help you understand the math behind the spline generation

    // Devide a circle in different size parts and generate a random point on a circle the size of each part
    int minNumCirlceParts = 6;
    int maxNumCirlceParts = 12;
    int fullNum = 1000;
    float curNum = 0.0f;
    float minInc = fullNum / maxNumCirlceParts;
    float maxInc = fullNum / minNumCirlceParts;
    int modInc = maxInc - minInc;
    float curInc = rand() % modInc + minInc;
    curNum += curInc;
    curInc += rand() % modInc + minInc;

    float radius = 1;
    while (curNum < fullNum) {
        float angle1 = curNum / fullNum * 2 * glm::pi<float>();
        float x1 = glm::cos(angle1) * radius;
        float y1 = glm::sin(angle1) * radius;
        float angle2 = (curNum + curInc) / fullNum * 2 * glm::pi<float>();
        float x2 = glm::cos(angle2) * radius;
        float y2 = glm::sin(angle2) * radius;

        // Create a new circle going through the points (x1, y1) and (x2, y2)
        float cx = (x1 + x2) / 2;
        float cy = (y1 + y2) / 2;

        // Distance between 1 and 2
        float d = glm::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

        // C is the center of a new circle with radius
        float r = d / 1.0f;

        // Calculate a random point on the circle
        float rangle = rand() / static_cast<float>(RAND_MAX) * 2 * glm::pi<float>();
        float x = cx + r * glm::cos(rangle);
        float y = cy + r * glm::sin(rangle);

        spline.addPoint(glm::vec2(x, y));

        curNum += curInc;
        curInc = rand() % modInc + minInc;
    }

    return spline;
}
