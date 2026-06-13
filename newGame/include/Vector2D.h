#pragma once

class Vector2D {
public:
    float x;
    float y;

    Vector2D(float x=0,float y=0);

    float magnitude() const;
    float angle() const;
};