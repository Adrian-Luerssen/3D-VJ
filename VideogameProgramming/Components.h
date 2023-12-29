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
    float ticks = 0;
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
        , const char* roughnessFilepath = "Textures/black.png", const char* emissiveFilepath = "Textures/black.png", const char* metallicFilepath = "Textures/black.png") {
        this->textureFilepath = textureFilepath;
        this->meshFilepath = meshFilepath;
        this->shaderName = shaderName;
        this->normalsFilepath = normalsFilepath;
    }
    const char* textureFilepath;
    const char* normalsFilepath;
    const char* meshFilepath;
    const char* shaderName;

    const char* roughnessFilepath = "Textures/black.png";
    const char* emissiveFilepath = "Textures/black.png";
    const char* metallicFilepath = "Textures/black.png";
    const char* mixedAO = "Textures/white.png";
};

struct Skybox
{
    Skybox(const char* background1, const char* foreground1, const char* foreground2, const char* foreground3, const char* meshFilepath) {
        this->meshFilepath = meshFilepath;
        this->background1 = background1;
        this->foreground1 = foreground1;
        this->foreground2 = foreground2;
        this->foreground3 = foreground3;
    }
    const char* meshFilepath;
    const char* background1;
    const char* foreground1;
    const char* foreground2;
    const char* foreground3;
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
    int lives = 5;
    bool imunity = false;
};
struct UserComponent
{
    bool hit = false;
    bool destroyed = false;
    int shipSpeed = 0;

    float apitch, ayaw;
    float ticks;
    float thrust;
};

struct EnemyComponent
{
    EnemyComponent(int points, int id, const char* name) {
        this->points = points;
        this->id = id;
        this->name = name;
    }
    const char* name;
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
    BulletComponent(glm::vec3 pos, glm::vec3 dir,bool userBullet = true) {
        this->pos = pos;
        this->dir = dir;
        this->userBullet = userBullet;
    }

    glm::vec3 pos;
    glm::vec3 dir;
    bool userBullet;

};

struct TemporaryComponent {
    TemporaryComponent(glm::vec3 dir = glm::vec3(0.0f), float duration = 0.0f) {
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

struct UIComponent {
    UIComponent(const char* id, const char* page) {
        this->id = id;
        this->page = page;
    }
    const char* id;
    const char* page;
};
struct ButtonComponent {
    ButtonComponent(const char* id, const char* frame1, const char* frame2, const char* frame3, const char* frame4, const char* frame5) {
		this->id = id;
		this->frame1 = frame1;
		this->frame2 = frame2;
		this->frame3 = frame3;
		this->frame4 = frame4;
		this->frame5 = frame5;
	}
	const char* id;
	const char* frame1;
	const char* frame2;
	const char* frame3;
	const char* frame4;
	const char* frame5;
    float ticksSinceClicked = 0;
    bool clicked = false;

};
struct SoundComponent {
    SoundComponent(const char* filePath,float volume = 1.0f,bool stackable = true) {
		this->filePath = filePath;
        this->volume = volume;
        this->stackable = stackable;
	}
	const char* filePath;
    bool stackable;
    bool cancel = false;
	bool playSound = false;
    float volume;
};

struct EnemyShipCanon
{
    EnemyShipCanon(glm::vec3 lowRotBound, glm::vec3 highRotBound, int shipID) {
        this->highRotBound = highRotBound;
        this->lowRotBound = lowRotBound;
        this->shipID = shipID;
    }
    int shipID;
    glm::vec3 lowRotBound;
    glm::vec3 highRotBound;
    float ticksToFire = std::rand() % 100;
    float ticks = 0;
};

struct HeartComponent {
    HeartComponent(int heartID) {
        this->heartID = heartID;
    }
    int heartID;
};