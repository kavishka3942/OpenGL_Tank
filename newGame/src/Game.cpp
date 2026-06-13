#include "../include/Game.h"
#include <cmath>
#include <algorithm>
#include <GL/freeglut.h>

Game game;

Game::Game()
{
    mouseX = 0;
    mouseY = 0;

    camX = 0;
    camY = 0;

    for(int i = 0; i < 256; i++)
        keys[i] = false;

    // obstacles
    obstacles.push_back(Obstacle(300, 200, 40));
    obstacles.push_back(Obstacle(500, 300, 40));
    obstacles.push_back(Obstacle(200, 400, 40));
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
    // rotation
    float worldMouseX = mouseX + camX;
    float worldMouseY = mouseY + camY;

    float dx = worldMouseX - player.pos.x;
    float dy = worldMouseY - player.pos.y;

player.angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    player.angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    // movement
    float speed = 2.5f;

    float nextX = player.pos.x;
    float nextY = player.pos.y;

    if(keys['w']) nextY -= speed;
    if(keys['s']) nextY += speed;
    if(keys['a']) nextX -= speed;
    if(keys['d']) nextX += speed;

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

    // camera (viewport)
    camX = player.pos.x - 400;
    camY = player.pos.y - 300;

    // bullets
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
    // ================= WORLD RENDER (CAMERA APPLIED) =================
    glPushMatrix();
    glTranslatef(-camX, -camY, 0);

    // obstacles
    for(auto &o : obstacles)
        o.draw();

    // bullets
    for(auto &b : bullets)
        b.draw();

    glPopMatrix();

    // ================= PLAYER (CENTERED VIEW) =================
    glPushMatrix();

    // keep player centered on screen
    glTranslatef(400 - player.pos.x, 300 - player.pos.y, 0);

    player.draw();

    glPopMatrix();
}