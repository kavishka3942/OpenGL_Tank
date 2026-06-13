#include "../include/Vector2D.h"
#include <cmath>

Vector2D::Vector2D(float x,float y)
{
    this->x=x;
    this->y=y;
}

float Vector2D::magnitude() const
{
    return sqrt(x*x+y*y);
}

float Vector2D::angle() const
{
    return atan2(y,x);
}