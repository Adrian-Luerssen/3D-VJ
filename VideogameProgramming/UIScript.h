#pragma once

#include "Script.h"

using namespace std;

class UIScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime) override;

private:
    void startButtonCheck(float deltaTime);
    bool firstClick = true;
    const char* page = "Menu";
    bool changePage = true;
    bool clearScreen = false;
};