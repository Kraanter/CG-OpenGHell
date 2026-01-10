#include "objectScene.h"

#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "texture.h"

ModelSpace::ModelSpace(): model(glm::mat4(1.0f)) {}

void ModelSpace::reset() { model = glm::mat4(1.0f); }

ModelSpace* ModelSpace::translate(glm::vec3 translation) {
    model = glm::translate(model, translation);
    return this;
}

ModelSpace* ModelSpace::rotate(float angle, glm::vec3 axis) {
    model = glm::rotate(model, angle, axis);
    return this;
}

ModelSpace* ModelSpace::scale(glm::vec3 scale) {
    model = glm::scale(model, scale);
    return this;
}

ModelSpace* ModelSpace::scale(float scale) {
    model = glm::scale(model, glm::vec3(scale));
    return this;
}

glm::vec3 createVector(float alpha, float beta) {
    float x = cos(beta) * sin(alpha);
    float y = sin(beta);
    float z = cos(beta) * cos(alpha);
    return glm::vec3(x, y, z);
}

glm::vec3 objectScene::calculateDirectionVector(bool withVertical) {
    if (!withVertical) { return createVector(cameraAlpha, 0.0f); }
    return createVector(cameraAlpha, cameraBeta);
}

void objectScene::rotateCamera(float deltaAlpha, float deltaBeta) {
    cameraAlpha += deltaAlpha;
    cameraBeta = std::clamp(cameraBeta + deltaBeta, -1.5f, 1.5f);
}

object::object(objectData data, Material* material) : data(data), material(material), modelSpace(ModelSpace()) {}

void object::bindVBO(GLuint program_id) { data.bindVBO(program_id); }

void object::render(const UniformVars* uVars, const glm::mat4* view, glm::vec3 light_pos) {
    glm::mat4 mv = *view * modelSpace.model;
    glBindTexture(GL_TEXTURE_2D, data.texture_id);
    glUniform3fv(uVars->uniform_material_ambient, 1, value_ptr(material->ambient_color));
    glUniform3fv(uVars->uniform_material_diffuse, 1, value_ptr(material->diffuse_color));
    glUniform3fv(uVars->uniform_specular, 1, value_ptr(material->specular_color));
    glUniform1f(uVars->uniform_material_power, material->power);
    glUniform1i(uVars->uniform_texture_only, material->use_texture_only ? 1 : 0);
    glUniform1i(uVars->uniform_panorama, material->use_panorama ? 1 : 0);
    glUniform1i(uVars->uniform_toon, material->use_toon ? 1 : 0);
    glUniformMatrix4fv(uVars->uniform_mv, 1, GL_FALSE, value_ptr(mv));

    glBindVertexArray(data.vao);
    glDrawArrays(GL_TRIANGLES, 0, data.vertices.size());
    glBindVertexArray(0);
}

void object::debugPrint() {
    std::cout << "UVs:\n";
    for (auto& uv : data.uvs) { std::cout << '(' << uv.x << ", " << uv.y << ')'; }
    std::cout << "\nNormals:\n";
    for (auto& normal : data.normals) { std::cout << '(' << normal.x << ", " << normal.y << ", " << normal.z << ')'; }
    std::cout << "\nVertices:\n";
    for (auto& vertex : data.vertices) { std::cout << '(' << vertex.x << ", " << vertex.y << ", " << vertex.z << ')'; }
    std::cout << '\n';
}

void objectScene::clearVBO() {
    for (auto& obj : objects)
        glDeleteVertexArrays(1, &obj.data.vao);
}

objectScene::objectScene(ApplicationData* app_data) {
    this->appData = app_data;
    objectScene::resetAndInit();
    skyboxRef = new skybox(500, "textures/Skybox.png");
};
 
object* objectScene::addObject(const char* obj_path, const char* txt_path, Material* material, bool visible) {
    objectData data(obj_path, txt_path);

    return addObject(data, material, visible);
}

object* objectScene::addObject(objectData data, Material* material, bool visible) {
    object obj(data, material);
    obj.visible = visible;
    objects.push_back(obj);
    num_objects++;

    return &objects[num_objects - 1];
}

object* objectScene::addGroundPlane(float size, const char* texture_path, float uv_scale) {
    (void)texture_path;
    (void)uv_scale;
    objectData ground_data;
    ground_data.texture_id = createSolidTexture(160, 160, 160);
    ground_data.vertices = {
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 1.0f),
    };
    ground_data.uvs = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),
    };
    ground_data.normals = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    };

    object* ground = addObject(ground_data, createMaterial());
    ground->modelSpace.scale(size);
    return ground;
}


void objectScene::render(glm::vec3 light_pos, bool paused) {
    if (!paused) {
        preRenderCallback(light_pos);
    }
    glm::mat4 view = currentViewMat();
    glm::vec3 sceneLight = light_pos;
    if (getLightPosition(sceneLight)) {
        glm::vec3 lightView = glm::vec3(view * glm::vec4(sceneLight, 1.0f));
        glUniform3fv(uniform_vars->uniform_light_pos, 1, value_ptr(lightView));
    } else {
        glm::vec3 lightView = normalize(glm::vec3(0.2f, 1.0f, 0.3f)) * 10000.0f;
        glUniform3fv(uniform_vars->uniform_light_pos, 1, value_ptr(lightView));
    }
    skyboxRef->objectRef->modelSpace.setLocation(cameraPos);
    skyboxRef->objectRef->render(uniform_vars, &view, light_pos);
    for (auto& obj : objects)
        if (obj.visible) {
            glm::mat4 curViewMat = currentViewMat();
            obj.render(uniform_vars, &curViewMat, light_pos);
        }
}

void objectScene::setUniformVars(UniformVars* uniform_vars, const GLuint program_id) {
    this->uniform_vars = uniform_vars;
    appData->program_id = program_id;

    skyboxRef->objectRef->bindVBO(program_id);
    for (auto& obj : objects)
        obj.bindVBO(program_id);
}

Material* objectScene::createMaterial() {
    const auto material = new Material();
    material->ambient_color = glm::vec3(0.18f, 0.18f, 0.18f);
    material->diffuse_color = glm::vec3(0.6f, 0.6f, 0.6f);
    material->specular_color = glm::vec3(0.4f, 0.4f, 0.4f);
    material->power = 50.0;
    return material;
}
