#pragma once

#include "Script.h"
#include "AsteroidScript.h"

using namespace std;

class BulletScript : public Script
{

    using Script::Script;
    

public:

    void startScript() override;

    void tickScript(float deltaTime) override;



private:
    virtual void CheckCollisions(Entity* entity);



};

class EnemyBulletScript : public Script
{

    using Script::Script;


public:


    void startScript() override;

    void tickScript(float deltaTime) override;



private:



};