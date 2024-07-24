#pragma once

#include <glm/glm.hpp>

class CatmullRom {
public:
    CatmullRom();
    CatmullRom(glm::vec2* points, unsigned numBeziers);
    void setPoints(glm::vec2* points, unsigned numBeziers);
    void addPoint(glm::vec2 p);
    void removePoint(int index);
    int getNumPoints();
    void setTension(float t);
    glm::vec2 getPoint(float t);

private:
    glm::vec2* points;
    unsigned numPoints;
    float tension;
    glm::vec2 getPoint(float t, int bezierIndex);
    glm::vec2 getTangent(float t, int bezierIndex);
    glm::vec2 getNormal(float t, int bezierIndex);
    glm::vec2 getBinormal(float t, int bezierIndex);
    glm::vec2 getCurvature(float t, int bezierIndex);
};
