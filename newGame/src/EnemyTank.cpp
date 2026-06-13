#include "../include/EnemyTank.h"
#include <GL/freeglut.h>
#include <cmath>

EnemyTank::EnemyTank(float x, float y)
{
    pos = Vector2D(x, y);
    angle = 0;
    alive = true;
    shootTimer = 0;
}

void EnemyTank::update(Vector2D playerPos)
{
    float dx = playerPos.x - pos.x;
    float dy = playerPos.y - pos.y;

    angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    shootTimer += 0.016f;
}

void EnemyTank::draw()
{
    if(!alive) return;

    glPushMatrix();
    glTranslatef(pos.x, pos.y, 0);
    glRotatef(angle, 0, 0, 1);

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
        glVertex2f(-20,-15);
        glVertex2f(20,-15);
        glVertex2f(20,15);
        glVertex2f(-20,15);
    glEnd();

    glColor3f(0.3f,0.3f,0.3f);
    glBegin(GL_QUADS);
        glVertex2f(0,-3);
        glVertex2f(30,-3);
        glVertex2f(30,3);
        glVertex2f(0,3);
    glEnd();

    glPopMatrix();
}

bool EnemyTank::checkHit(Vector2D bulletPos)
{
    float dx = bulletPos.x - pos.x;
    float dy = bulletPos.y - pos.y;

    return (sqrt(dx*dx + dy*dy) < 25);
}