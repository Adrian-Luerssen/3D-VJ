#pragma once

#include "Script.h"
#include <random>

using namespace std;

class UserScript : public Script
{

    using Script::Script;

public:

    void tickScript(float deltaTime) override;

    void Break();



private:
    void cameraShake(ComponentHandle<Camera> cam);

    float speed = 0.01f;
    float sensitivity = 100.0f;
    bool moveCam = false;
    bool firstClick = false;

    glm::vec3 up = glm::vec3(0., 1., 0.);
    bool shake = false;
    float timeShaking = 100.0f;

};