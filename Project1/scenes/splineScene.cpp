#include "splineScene.h"

#include <iostream>

splineScene::splineScene(ApplicationData* application_data): objectScene(application_data) {
    splineScene::resetAndInit();
}

glm::vec3 splineScene::startCameraPos() { return glm::vec3(0.1f, 10.0f, 0.0f); }

void splineScene::keyboardHandler(unsigned char key) {
    switch (key) {
    case ' ':
        lockedCamera = !lockedCamera;
        break;
    case 'l':
        // cameraRotationHor += 0.1f;
        // centerPos = calcCenterPos();
        break;
    case 'j':
        // cameraRotationHor -= 0.1f;
        // centerPos = calcCenterPos();
        break;
    case 'i':
        // cameraRotationVer += 0.1f;
        // centerPos = calcCenterPos();
        break;
    case 'k':
        // cameraRotationVer -= 0.1f;
        // centerPos = calcCenterPos();
        break;
    case 'e':
        cameraPos.y += 1.0f;
        break;
    case 'q':
        if (cameraPos.y - 1.0f < 0) { return; }
        cameraPos.y -= 1.0f;
        break;
    case 'p':
        inCarView = !inCarView;
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

    if (lockedCamera) {
        // Point the camera in the direction of the car and track the car
        // cameraPos = currentTrackPos() + glm::vec3(0.01, 1, 0);
    }

    if (car != nullptr) {
        carPos = currentTrackPos();
        carPos.y = 1.5f;
        glm::vec2 dir = currentTrackDir();
        if (glm::length(dir) > 1e-5f) {
            carYaw = std::atan2(dir.x, dir.y);
        }

        car->modelSpace.reset();
        car->modelSpace.translate(carPos)
            ->rotate(carYaw, glm::vec3(0.0f, 1.0f, 0.0f))
            ->scale(0.01f);
    }

    if (inCarView) {
        cameraPos = carPos + glm::vec3(0.0f, 1.0f, 0.0f);
        cameraAlpha = carYaw;
        cameraBeta = 0.0f;
    }
}

void splineScene::resetAndInit() {
    objectScene::resetAndInit();
    cameraPos = startCameraPos();

    std::cout << "[spline] resetAndInit start" << std::endl;
    addGroundPlane(2000.0f);
    std::cout << "[spline] generating track spline" << std::endl;
    trackSpline = generateTrackSpline();
    std::cout << "[spline] compiling track mesh" << std::endl;
    compileTrack();
    std::cout << "[spline] track mesh compiled" << std::endl;

    auto* carMaterial = createMaterial();
    carMaterial->use_toon = true;
    carMaterial->diffuse_color = appData->getSelectedCarColor();
    car = addObject(appData->getSelectedCarObj().c_str(), appData->getSelectedCarTxt().c_str(), carMaterial, true);
    car->modelSpace.scale(0.01f);
    carPos = currentTrackPos();
    carPos.y = 1.5f;
    carYaw = 0.0f;
    std::cout << "[spline] resetAndInit done" << std::endl;
}

#define TRACKWIDTH 3.0f

void splineScene::compileTrack() {
    // Create a track from the spline
    constexpr int numPoints = 1600;
    constexpr float trackY = 1.5f;

    objectData data;
    data.setTexture("textures/track.bmp");

    std::cout << "[spline] compileTrack samples=" << numPoints << std::endl;
    auto first = trackSpline.getPoint(1.0f / numPoints);
    glm::vec2 last = first;

    for (float i = 0; i < numPoints - 1; i += 1.0f) {
        float t1 = i / (numPoints - 1.0f);
        glm::vec2 p1 = trackSpline.getPoint(t1);
        if (glm::length(p1 - last) > 1e-4f) {
            glm::vec2 dir = normalize(p1 - last);
            auto dir3d = glm::vec3(dir.x, dir.y, 0.0f);
            auto crossDir3d = normalize(cross(dir3d, glm::vec3(0.0f, 0.0f, 10.0f)));
            auto crossDir = glm::vec2(crossDir3d.x, crossDir3d.y);

            auto p3 = last + crossDir * TRACKWIDTH;
            auto p4 = p1 + crossDir * TRACKWIDTH;
            auto p5 = p1 - crossDir * TRACKWIDTH;
            auto p6 = last - crossDir * TRACKWIDTH;

            data.vertices.insert(data.vertices.end(), {
                glm::vec3(p3.x, trackY, p3.y),
                glm::vec3(p4.x, trackY, p4.y),
                glm::vec3(p5.x, trackY, p5.y),
                glm::vec3(p3.x, trackY, p3.y),
                glm::vec3(p5.x, trackY, p5.y),
                glm::vec3(p6.x, trackY, p6.y),
            });
            data.uvs.insert(data.uvs.end(), {
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
            });
            data.normals.insert(data.normals.end(), {
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
            });
        }
        last = p1;
    }

    if (glm::length(last - first) > 1e-4f) {
        glm::vec2 dir = normalize(first - last);
        auto dir3d = glm::vec3(dir.x, dir.y, 0.0f);
        auto crossDir3d = normalize(cross(dir3d, glm::vec3(0.0f, 0.0f, 10.0f)));
        auto crossDir = glm::vec2(crossDir3d.x, crossDir3d.y);

        auto p3 = last + crossDir * TRACKWIDTH;
        auto p4 = first + crossDir * TRACKWIDTH;
        auto p5 = first - crossDir * TRACKWIDTH;
        auto p6 = last - crossDir * TRACKWIDTH;

        data.vertices.insert(data.vertices.end(), {
            glm::vec3(p3.x, trackY, p3.y),
            glm::vec3(p4.x, trackY, p4.y),
            glm::vec3(p5.x, trackY, p5.y),
            glm::vec3(p3.x, trackY, p3.y),
            glm::vec3(p5.x, trackY, p5.y),
            glm::vec3(p6.x, trackY, p6.y),
        });
        data.uvs.insert(data.uvs.end(), {
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
        });
        data.normals.insert(data.normals.end(), {
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
        });
    }

    std::cout << "[spline] track triangles=" << data.vertices.size() / 3 << std::endl;
    addObject(data, createMaterial());
}

void splineScene::createTrackPart(glm::vec2 p1, glm::vec2 p2) {
    if (glm::length(p2 - p1) < 1e-4f) { return; }
    glm::vec2 dir = normalize(p2 - p1);
    float angle = glm::atan(dir.y, dir.x);
    float length = glm::length(p2 - p1);

    auto dir3d = glm::vec3(dir.x, dir.y, 0.0f);
    auto crossDir3d = normalize(cross(dir3d, glm::vec3(0.0f, 0.0f, 10.0f)));
    auto crossDir = glm::vec2(crossDir3d.x, crossDir3d.y);

    auto p3 = p1 + crossDir * TRACKWIDTH;
    auto p4 = p2 + crossDir * TRACKWIDTH;
    auto p5 = p2 - crossDir * TRACKWIDTH;
    auto p6 = p1 - crossDir * TRACKWIDTH;

    auto data = objectData();
    data.setTexture("textures/track.bmp");
    data.vertices = {
        // 6 points to create 2 triangles to create a quad p3-p4-p5-p6
        glm::vec3(p3.x, 1.5f, p3.y),
        glm::vec3(p4.x, 1.5f, p4.y),
        glm::vec3(p5.x, 1.5f, p5.y),
        glm::vec3(p3.x, 1.5f, p3.y),
        glm::vec3(p5.x, 1.5f, p5.y),
        glm::vec3(p6.x, 1.5f, p6.y),

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
    constexpr int controlPoints = 16;
    constexpr float baseRadius = 120.0f;
    constexpr float radiusJitter = 50.0f;
    constexpr float angleJitter = 0.5f;

    std::vector<float> deltas;
    deltas.reserve(controlPoints);
    float sum = 0.0f;
    for (int i = 0; i < controlPoints; ++i) {
        float base = glm::two_pi<float>() / static_cast<float>(controlPoints);
        float jitter = (rand() / static_cast<float>(RAND_MAX)) * angleJitter - (angleJitter * 0.5f);
        float delta = base * (1.0f + jitter);
        if (delta < base * 0.3f) { delta = base * 0.3f; }
        deltas.push_back(delta);
        sum += delta;
    }

    auto* points = new glm::vec2[controlPoints];
    float angle = 0.0f;
    for (int i = 0; i < controlPoints; ++i) {
        angle += deltas[i] * (glm::two_pi<float>() / sum);
        float jitter = (rand() / static_cast<float>(RAND_MAX)) * radiusJitter;
        float r = baseRadius + jitter;
        float x = glm::cos(angle) * r;
        float y = glm::sin(angle) * r;
        points[i] = glm::vec2(x, y);
    }
    spline.setPoints(points, controlPoints);
    std::cout << "[spline] control points=" << controlPoints << std::endl;

    return spline;
}

void splineScene::onSelectedCarChanged() {
    clearVBO();
    resetAndInit();
    for (auto& obj : objects) { obj.bindVBO(appData->program_id); }
}

void splineScene::onCarColorChanged() {
    if (!objects.empty()) {
        car = &objects.back();
    }
    if (car != nullptr) {
        car->material->diffuse_color = appData->getSelectedCarColor();
    }
}
