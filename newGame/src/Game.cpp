#include "../include/Game.h"
#include <cmath>
#include <algorithm>

Game game;

Game::Game()
{
    mouseX = 0;
    mouseY = 0;

    for(int i = 0; i < 256; i++)
        keys[i] = false;

    // ---------------- CREATE OBSTACLES ----------------
    obstacles.push_back(Obstacle(300, 200, 40));
    obstacles.push_back(Obstacle(500, 300, 40));
    obstacles.push_back(Obstacle(200, 400, 40));
}

void Game::setInput(bool inputKeys[256])
{
    for(int i = 0; i < 256; i++)
        keys[i] = inputKeys[i];
}

// ---------------- SHOOT ----------------
void Game::shoot()
{
    bullets.push_back(
        Projectile(player.pos.x, player.pos.y, player.angle)
    );
}

void Game::update()
{
    // rotation
    float dx = mouseX - player.pos.x;
    float dy = mouseY - player.pos.y;

    player.angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    // ---------------- MOVEMENT ----------------
    float nextX = player.pos.x;
    float nextY = player.pos.y;

    if(keys['w']) nextY -= player.speed;
    if(keys['s']) nextY += player.speed;
    if(keys['a']) nextX -= player.speed;
    if(keys['d']) nextX += player.speed;

    // ---------------- COLLISION (PLAYER vs OBSTACLE) ----------------
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

    // ---------------- BULLETS ----------------
    for(auto &b : bullets)
    {
        b.update();

        // bullet vs obstacle collision
        for(auto &o : obstacles)
        {
            if(o.checkCollision(b.pos.x, b.pos.y))
            {
                b.active = false;
            }
        }
    }

    // remove inactive bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
        [](Projectile &b){ return !b.active; }),
        bullets.end()
    );
}

void Game::draw()
{
    // draw obstacles first
    for(auto &o : obstacles)
        o.draw();

    // draw bullets
    for(auto &b : bullets)
        b.draw();

    // draw player
    player.draw();
}