#pragma once

#include "Vector2D.h"

class Obstacle
{
public:
    Vector2D pos;
    float size;

    Obstacle(float x, float y, float s);

    void draw();

    bool checkCollision(float x, float y);
};