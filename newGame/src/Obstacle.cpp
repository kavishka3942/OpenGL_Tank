#include "../include/Obstacle.h"
#include <GL/freeglut.h>

Obstacle::Obstacle(float x, float y, float s)
{
    pos = Vector2D(x, y);
    size = s;
}

void Obstacle::draw()
{
    glColor3f(0.6f, 0.3f, 0.1f);

    glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + size, pos.y);
        glVertex2f(pos.x + size, pos.y + size);
        glVertex2f(pos.x, pos.y + size);
    glEnd();
}

// point-in-square collision
bool Obstacle::checkCollision(float x, float y)
{
    return (x >= pos.x &&
            x <= pos.x + size &&
            y >= pos.y &&
            y <= pos.y + size);
}