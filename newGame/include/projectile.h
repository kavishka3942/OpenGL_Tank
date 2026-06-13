#pragma once

#include "Vector2D.h"

class Projectile
{
public:
    Vector2D pos;
    Vector2D velocity;

    float speed;
    bool active;

    Projectile(float x, float y, float angle);

    void update();
    void draw();
};