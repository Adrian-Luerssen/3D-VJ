#pragma once

#include "Script.h"

class SpawnerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime) override;

private:

    float delay = 500.f;
    float t = 0.;
    int counter = 0;
    bool firstClick = true;
    bool firstSpawn = true;

};
