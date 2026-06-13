#include "../include/Projectile.h"
#include <GL/freeglut.h>
#include <cmath>

Projectile::Projectile(float x, float y, float angle, bool enemy)
{
    pos = Vector2D(x, y);

    float rad = angle * 3.14159265f / 180.0f;

    velocity.x = cos(rad);
    velocity.y = sin(rad);

    speed = 6.0f;
    active = true;
    fromEnemy = enemy;
}

void Projectile::update()
{
    pos.x += velocity.x * speed;
    pos.y += velocity.y * speed;
}

void Projectile::draw()
{
    if(fromEnemy)
        glColor3f(1.0f, 1.0f, 0.0f);   // yellow enemy bullets
    else
        glColor3f(1.0f, 0.2f, 0.0f);   // orange player bullets

    glBegin(GL_QUADS);
        glVertex2f(pos.x - 3, pos.y - 3);
        glVertex2f(pos.x + 3, pos.y - 3);
        glVertex2f(pos.x + 3, pos.y + 3);
        glVertex2f(pos.x - 3, pos.y + 3);
    glEnd();
}