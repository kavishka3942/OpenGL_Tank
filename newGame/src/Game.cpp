#include "../include/Game.h"
#include <cmath>
#include <algorithm>
#include <GL/freeglut.h>
#include <cstdlib>

Game game;

Game::Game()
{
    mouseX = 0;
    mouseY = 0;

    camX = 0;
    camY = 0;

    for(int i = 0; i < 256; i++)
        keys[i] = false;

    // ---------------- RANDOM OBSTACLES (80) ----------------
    for(int i = 0; i < 80; i++)
    {
        float x = rand() % 1800 + 50;
        float y = rand() % 1800 + 50;

        obstacles.push_back(Obstacle(x, y, 40));
    }

    // optional: ensure player starts inside world
    player.pos.x = 100;
    player.pos.y = 100;
}

void Game::setInput(bool inputKeys[256])
{
    for(int i = 0; i < 256; i++)
        keys[i] = inputKeys[i];
}

void Game::shoot()
{
    bullets.push_back(
        Projectile(player.pos.x, player.pos.y, player.angle)
    );
}

void Game::update()
{
    // ================= WORLD BOUNDARY =================
    const float WORLD_W = 2000;
    const float WORLD_H = 2000;

    float speed = 2.5f;

    float nextX = player.pos.x;
    float nextY = player.pos.y;

    if(keys['w']) nextY -= speed;
    if(keys['s']) nextY += speed;
    if(keys['a']) nextX -= speed;
    if(keys['d']) nextX += speed;

    // ---------------- CLAMP PLAYER INSIDE WORLD ----------------
    if(nextX < 20) nextX = 20;
    if(nextX > WORLD_W - 20) nextX = WORLD_W - 20;
    if(nextY < 20) nextY = 20;
    if(nextY > WORLD_H - 20) nextY = WORLD_H - 20;

    // ---------------- OBSTACLE COLLISION ----------------
    bool canMove = true;

    for(auto &o : obstacles)
    {
        if(o.checkCollision(nextX, nextY))
        {
            canMove = false;
            break;
        }
    }

    if(canMove)
    {
        player.pos.x = nextX;
        player.pos.y = nextY;
    }

    // ================= CAMERA =================
    camX = player.pos.x - 400;
    camY = player.pos.y - 300;

    // clamp camera inside world
    if(camX < 0) camX = 0;
    if(camY < 0) camY = 0;
    if(camX > WORLD_W - 800) camX = WORLD_W - 800;
    if(camY > WORLD_H - 600) camY = WORLD_H - 600;

    // ================= MOUSE ROTATION =================
    float worldMouseX = mouseX + camX;
    float worldMouseY = mouseY + camY;

    float dx = worldMouseX - player.pos.x;
    float dy = worldMouseY - player.pos.y;

    player.angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    // ================= BULLETS =================
    for(auto &b : bullets)
    {
        b.update();

        for(auto &o : obstacles)
        {
            if(o.checkCollision(b.pos.x, b.pos.y))
                b.active = false;
        }
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
        [](Projectile &b){ return !b.active; }),
        bullets.end()
    );
}

void Game::draw()
{
    // ---------------- BACKGROUND (OUTSIDE WORLD = WHITE) ----------------
    glClearColor(0.9f, 0.8f, 0.6f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    const float WORLD_W = 2000;
    const float WORLD_H = 2000;

    glPushMatrix();

    // ---------------- CAMERA ----------------
    glTranslatef(-camX, -camY, 0);

    // ---------------- WORLD BOUNDARY ----------------
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0,0);
        glVertex2f(WORLD_W,0);
        glVertex2f(WORLD_W,WORLD_H);
        glVertex2f(0,WORLD_H);
    glEnd();

    // ---------------- OBSTACLES ----------------
    for(auto &o : obstacles)
        o.draw();

    // ---------------- BULLETS ----------------
    for(auto &b : bullets)
        b.draw();

    // ---------------- PLAYER ----------------
    player.draw();

    glPopMatrix();
}