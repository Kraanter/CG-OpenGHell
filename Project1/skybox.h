#pragma once

struct Material;
class object;

class skybox {
public:
    object* objectRef;
    skybox(int size, const char* texture_path = nullptr);
    object* createSkyboxData(int size, const char* texture_path);

protected:
    static Material* createMaterial();
};
