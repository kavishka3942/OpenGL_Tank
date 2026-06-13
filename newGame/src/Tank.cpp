#include "../include/Tank.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/freeglut.h>

Tank::Tank()
{
    pos = Vector2D(400,300);
    velocity = Vector2D(0,0);

    angle = 0;
    speed = 3.0f;
}

void Tank::draw()
{
    glPushMatrix();

    glTranslatef(pos.x,pos.y,0);

    glRotatef(angle,0,0,1);

    glColor3f(0,0,1);

    glBegin(GL_QUADS);

        glVertex2f(-20,-15);
        glVertex2f(20,-15);
        glVertex2f(20,15);
        glVertex2f(-20,15);

    glEnd();

    glColor3f(0.5f,0.5f,0.5f);

    glBegin(GL_QUADS);

        glVertex2f(0,-3);
        glVertex2f(30,-3);
        glVertex2f(30,3);
        glVertex2f(0,3);

    glEnd();

    glPopMatrix();
}

void Tank::update()
{
    pos.x += velocity.x * speed;
    pos.y += velocity.y * speed;
}