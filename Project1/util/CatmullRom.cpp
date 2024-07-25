#include "CatmullRom.h"

CatmullRom::CatmullRom() {
    numPoints = 0;
    points = nullptr;
}

CatmullRom::CatmullRom(glm::vec2* points, unsigned numBeziers) { setPoints(points, numBeziers); }

void CatmullRom::setPoints(glm::vec2* points, unsigned numBeziers) {
    this->points = points;
    this->numPoints = numBeziers;
}

void CatmullRom::addPoint(glm::vec2 p) {
    auto newPoints = new glm::vec2[numPoints + 1];
    for (int i = 0; i < numPoints; i++) { newPoints[i] = points[i]; }
    newPoints[numPoints] = p;
    numPoints++;
    delete[] points;
    points = newPoints;
}

void CatmullRom::removePoint(int index) {
    auto newPoints = new glm::vec2[numPoints - 1];
    for (int i = 0; i < index; i++) { newPoints[i] = points[i]; }
    for (int i = index + 1; i < numPoints; i++) { newPoints[i - 1] = points[i]; }
    numPoints--;
    delete[] points;
    points = newPoints;
}

int CatmullRom::getNumPoints() { return numPoints; }

glm::vec2 CatmullRom::getDerivative(float t) {
    int bezierIndex = t * numPoints;
    float localT = t * numPoints - bezierIndex;
    return getTangent(localT, bezierIndex);
}

void CatmullRom::setTension(float t) { tension = t; }

glm::vec2 CatmullRom::getPoint(float t) {
    int bezierIndex = t * numPoints;
    float localT = t * numPoints - bezierIndex;
    return getPoint(localT, bezierIndex);
}

glm::vec2 CatmullRom::getPoint(float t, int bezierIndex) {
    glm::vec2 p0 = points[(bezierIndex - 1 + numPoints) % numPoints];
    glm::vec2 p1 = points[bezierIndex];
    glm::vec2 p2 = points[(bezierIndex + 1) % numPoints];
    glm::vec2 p3 = points[(bezierIndex + 2) % numPoints];

    float t2 = t * t;
    float t3 = t2 * t;

    glm::vec2 p = 0.5f * (
        (2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
    );

    return p;
}

glm::vec2 CatmullRom::getTangent(float t, int bezierIndex) {
    glm::vec2 p0 = points[(bezierIndex - 1 + numPoints) % numPoints];
    glm::vec2 p1 = points[bezierIndex];
    glm::vec2 p2 = points[(bezierIndex + 1) % numPoints];
    glm::vec2 p3 = points[(bezierIndex + 2) % numPoints];

    float t2 = t * t;

    glm::vec2 p = 0.5f * (
        (-p0 + p2) +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * 2.0f * t +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * 3.0f * t2
    );

    return p;
}

glm::vec2 CatmullRom::getNormal(float t, int bezierIndex) {
    glm::vec2 tangent = getTangent(t, bezierIndex);
    return normalize(glm::vec2(-tangent.y, tangent.x));
}

glm::vec2 CatmullRom::getBinormal(float t, int bezierIndex) {
    glm::vec2 tangent = getTangent(t, bezierIndex);
    glm::vec2 normal = getNormal(t, bezierIndex);
    return normalize(
        glm::vec2(tangent.x * normal.y - tangent.y * normal.x, tangent.y * normal.x - tangent.x * normal.y));
}

glm::vec2 CatmullRom::getCurvature(float t, int bezierIndex) {
    glm::vec2 tangent = getTangent(t, bezierIndex);
    glm::vec2 normal = getNormal(t, bezierIndex);
    glm::vec2 binormal = getBinormal(t, bezierIndex);
    return tangent / dot(tangent, tangent);
}
