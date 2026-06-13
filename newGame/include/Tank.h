#pragma once

#include "Vector2D.h"

class Tank
{
public:

    Vector2D pos;
    Vector2D velocity;

    float angle;
    float speed;

    Tank();

    void update();
    void draw();
};