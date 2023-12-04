#pragma once

#include "Script.h"
#include <random>

using namespace std;

class AsteroidScript : public Script
{

    using Script::Script;

public:

    void tickScript(float deltaTime) override;

    void Break();



private:
    void generateStartPos(int level, float renderDistance, glm::vec3& userPos);


    int hp = 1;
    bool destroyed = false;
    glm::vec3 dir = glm::vec3((-1.0 + static_cast<double>(rand() * 2.0) / (RAND_MAX)) / 1);
    float ticksBeforeAppear = rand()%5000;
    float ticksSinceDestroyed = 0;
    glm::vec3 startPos = glm::vec3(1000.0f); // Generates a random number between 0 and 800 (inclusive)
    glm::vec2 limits = glm::vec2(800., 800.);
    int asteroid_sprite = (rand() % 2) + 1;

};