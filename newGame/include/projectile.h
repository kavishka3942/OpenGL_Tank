#pragma once

#include "Vector2D.h"

class Projectile
{
public:
    Vector2D pos;
    Vector2D velocity;

    float speed;
    bool active;

    bool fromEnemy;   // NEW

    Projectile(float x, float y, float angle, bool enemy = false);

    void update();
    void draw();
};