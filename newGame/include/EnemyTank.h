#pragma once

#include "Vector2D.h"

class EnemyTank
{
public:
    Vector2D pos;
    float angle;
    bool alive;

    float shootTimer;

    EnemyTank(float x, float y);

    void update(Vector2D playerPos);
    void draw();

    bool checkHit(Vector2D bulletPos);
};