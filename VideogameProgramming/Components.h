#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

struct Transform2D
{
    Transform2D(glm::vec2 position, float rotation, float scale) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }

    glm::vec2 position;
    float rotation;
    float scale;
};

struct Transform3D
{

    Transform3D(glm::vec3 position, float scale, float rotation = 0) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
    float rotation;
    glm::vec3 position;
    float scale;
};

struct Camera
{
    Camera(glm::vec3 position, glm::vec3 orientation, glm::vec3 up) {
        this->position = position;
        this->orientation = orientation;
        this->up = up;
    }

    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 up;
};

struct Sprite
{
    Sprite(const char* filepath, glm::vec3 color, bool autoSize = true, glm::vec2 size = glm::vec2(1, 1), const char* shaderName = "default") {
        this->filepath = filepath;
        this->color = color;
        this->autoSize = autoSize;
        this->size = size;
        this->shaderName = shaderName;
    }

    const char* filepath;
    glm::vec3 color;
    bool autoSize;
    glm::vec2 size = glm::vec2(1, 1);
    const char* shaderName;
};

struct MeshComponent
{
    MeshComponent(const char* textureFilepath, const char* meshFilepath, const char* shaderName = "default", const char* normalsFilepath = "Textures/flat_normal.png") {
        this->textureFilepath = textureFilepath;
        this->meshFilepath = meshFilepath;
        this->shaderName = shaderName;
        this->normalsFilepath = normalsFilepath;
    }
    const char* textureFilepath;
    const char* normalsFilepath;
    const char* meshFilepath;
    const char* shaderName;
};

struct Skybox
{
    Skybox(const char* textureFilepath, const char* meshFilepath) {
        this->textureFilepath = textureFilepath;
        this->meshFilepath = meshFilepath;
    }
    const char* textureFilepath;
    const char* meshFilepath;
};

struct BoxCollider
{
    BoxCollider(float w, float h) {
        this->width = w;
        this->height = h;
    }

    float width = 0;
    float height = 0;
    bool collidedWith = false;
};

struct CubeCollider
{
    CubeCollider(float w, float h, float l) {
        this->width = w;
        this->height = h;
        this->length = l;
    }

    float width = 0;
    float height = 0;
    float length = 0;
    bool collidedWith = false;
};

struct ScriptComponent
{
    ScriptComponent(int scriptId) {
        this->scriptId = scriptId;
    }

    int scriptId = -1;
};

//----------------- My Components --------------------
struct UserComponent
{
    float renderDistance = 2000.0f;
    int lives = 0;
    bool hit = false;
    bool destroyed = false;
    int shipSpeed = 0;
};

struct EnemyComponent
{
    EnemyComponent(int points, int level, int id) {
        this->points = points;
        this->startLevel = level;
        this->level = level;
        this->id = id;
    }
    int points = 0;
    int level = 0;
    int startLevel = 0;
    bool destroyed = false;
    int id = 0;
};
struct BulletComponent
{
    BulletComponent(glm::vec3 pos, glm::vec3 dir) {
        this->pos=pos;
        this->dir=dir;
    }
 
    glm::vec3 pos;
    glm::vec3 dir;

};