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

    Transform3D(glm::vec3 position, float scale, glm::vec3 rotation = glm::vec3(0.0f)) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
    glm::vec3 rotation;
    glm::vec3 position;
    float scale;
};

struct Camera
{
    Camera(glm::vec3 eye, glm::vec3 front, glm::vec3 up, float renderDistance = 2000.0f) {
        this->eye = eye;
        this->front = front;
        this->up = up;
        this->renderDistance = renderDistance;
        this->target = front;
    }


    float renderDistance;
    float apitch;
    float ayaw;
    glm::vec3 eye;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 target;
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
    MeshComponent(const char* textureFilepath, const char* meshFilepath, const char* shaderName = "default", const char* normalsFilepath = "Textures/flat_normal.png"
        , const char* roughnessFilepath = "none", const char* emissiveFilepath = "none", const char* metallicFilepath = "none") {
        this->textureFilepath = textureFilepath;
        this->meshFilepath = meshFilepath;
        this->shaderName = shaderName;
        this->normalsFilepath = normalsFilepath;
    }
    const char* textureFilepath;
    const char* normalsFilepath;
    const char* meshFilepath;
    const char* shaderName;

    const char* roughnessFilepath;
    const char* emissiveFilepath;
    const char* metallicFilepath;
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
struct GameController {
    bool pause = true;
    bool freeCam = false;
    int score = 0;
};
struct UserComponent
{
    int lives = 0;
    bool hit = false;
    bool destroyed = false;
    int shipSpeed = 0;

    float apitch, ayaw;
    float ticks;
    float thrust;
};

struct EnemyComponent
{
    EnemyComponent(int points, int id) {
        this->points = points;
        this->id = id;
    }
    int points = 0;
    int id = 0;
    bool spawned = false;
    glm::vec3 dir;
    glm::vec3 startPos;
    bool destroyed = false;
    float ticksSinceDestroyed = 0;
};
struct BulletComponent
{
    BulletComponent(glm::vec3 pos, glm::vec3 dir) {
        this->pos = pos;
        this->dir = dir;
    }

    glm::vec3 pos;
    glm::vec3 dir;

};

struct TemporaryComponent {
    TemporaryComponent(glm::vec3 dir, float duration) {
        this->dir = dir;
        this->duration = duration;
    }
    glm::vec3 dir;
    float duration;
    float timeAlive = 0;
};
struct ScoreComponent {
    ScoreComponent(int digit) {
		this->digit = digit;
	}
    int digit;
};