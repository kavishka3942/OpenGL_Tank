#include "../include/Projectile.h"
#include <GL/freeglut.h>
#include <cmath>

Projectile::Projectile(float x, float y, float angle)
{
    pos = Vector2D(x, y);

    float rad = angle * 3.14159265f / 180.0f;

    velocity.x = cos(rad);
    velocity.y = sin(rad);

    speed = 6.0f;
    active = true;
}

void Projectile::update()
{
    pos.x += velocity.x * speed;
    pos.y += velocity.y * speed;
}

void Projectile::draw()
{
    glColor3f(1.0f, 0.2f, 0.0f);

    glBegin(GL_QUADS);
        glVertex2f(pos.x - 3, pos.y - 3);
        glVertex2f(pos.x + 3, pos.y - 3);
        glVertex2f(pos.x + 3, pos.y + 3);
        glVertex2f(pos.x - 3, pos.y + 3);
    glEnd();
}