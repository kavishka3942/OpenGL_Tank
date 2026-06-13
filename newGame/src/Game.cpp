#include "../include/Game.h"
#include <cmath>

Game game;

Game::Game()
{
    mouseX = 0;
    mouseY = 0;

    for(int i = 0; i < 256; i++)
        keys[i] = false;
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

    // movement
    player.velocity.x = 0;
    player.velocity.y = 0;

    if(keys['w']) player.velocity.y = -1;
    if(keys['s']) player.velocity.y = 1;
    if(keys['a']) player.velocity.x = -1;
    if(keys['d']) player.velocity.x = 1;

    player.update();

    // update bullets
    for(auto &b : bullets)
        b.update();
}

void Game::draw()
{
    player.draw();

    for(auto &b : bullets)
        b.draw();
}