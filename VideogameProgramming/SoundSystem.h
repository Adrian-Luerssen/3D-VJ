#pragma once

#include<iostream>
#include<string>
#include<map>
#include "ECS.h"
#include "Components.h"
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


using namespace irrklang;
using namespace std;
using namespace ECS;

class SoundSystem : public EntitySystem
{
public:

    SoundSystem();
    virtual void tick(World* world, float deltaTime) override;

private:
    ISoundEngine* soundEngine;

};
