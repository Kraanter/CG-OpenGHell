#include "splineScene.h"

#include <iostream>

splineScene::splineScene() { resetAndInit(); }

glm::vec3 splineScene::startCameraPos() { return glm::vec3(0, 0, 10); }

void splineScene::keyboardHandler(unsigned char key) {
    switch (key) {
    default:
        break;
    }
}

void splineScene::resetAndInit() {
    spline = CatmullRom();

    // Devide a circle in N equal parts and generate a random point in each part
    int numCirlceParts = 10;
    float radius = 5;
    for (int i = 0; i < numCirlceParts; i++) {
        float angle1 = static_cast<float>(i) / numCirlceParts * 2 * glm::pi<float>();
        float x1 = glm::cos(angle1) * radius;
        float y1 = glm::sin(angle1) * radius;
        float angle2 = static_cast<float>(i + 1) / numCirlceParts * 2 * glm::pi<float>();
        float x2 = glm::cos(angle2) * radius;
        float y2 = glm::sin(angle2) * radius;

        // Create a new circle going through the points (x1, y1) and (x2, y2)
        float cx = (x1 + x2) / 2;
        float cy = (y1 + y2) / 2;

        // Distance between 1 and 2
        float d = glm::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

        // C is the center of a new circle with radius d / 2
        float r = d / 2;

        // Calculate a random point on the circle
        float rangle = rand() / static_cast<float>(RAND_MAX) * 2 * glm::pi<float>();
        float x = cx + r * glm::cos(rangle);
        float y = cy + r * glm::sin(rangle);

        spline.addPoint(glm::vec2(x, y));
    }


    // Print a list of points
    int numPoints = 250;
    for (int i = 0; i < numPoints - 1; i++) {
        float t = static_cast<float>(i) / (numPoints - 1.0f);
        glm::vec2 p = spline.getPoint(t);
        std::cout << "(" << p.x << ", " << p.y << ")";
        if (i < numPoints - 2) { std::cout << ","; }
    }
    std::cout << std::endl;

    // Print list of all spline control points
    int numControlPoints = spline.getNumPoints();
    for (int i = 0; i < numControlPoints; i++) {
        glm::vec2 p = spline.getPoint(static_cast<float>(i) / numControlPoints);
        std::cout << "(" << p.x << ", " << p.y << ")";
        if (i < numControlPoints - 1) { std::cout << ","; }
    }
}
