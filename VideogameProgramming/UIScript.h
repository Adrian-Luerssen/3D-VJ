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
    bool firstClick = true;
    const char* page = "Menu";
    bool changePage = true;
};