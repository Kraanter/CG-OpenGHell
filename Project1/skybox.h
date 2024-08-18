#pragma once

struct Material;
class object;

class skybox {
public:
    object* objectRef;
    skybox(int size);
    object* createSkyboxData(int size);

protected:
    static Material* createMaterial();
};
