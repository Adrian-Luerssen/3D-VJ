#pragma once

#include "Script.h"

using namespace std;

class FirstPersonCameraScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime) override;

private:

    glm::vec3 direction = glm::vec3(1., 0., 0.);

    bool firstClick = true;
    float speed = 0.01f;
    float sensitivity = 100.0f;

};