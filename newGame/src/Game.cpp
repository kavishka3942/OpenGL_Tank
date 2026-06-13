#include "../include/Game.h"

#include <cmath>

Game game;

Game::Game()
{
    mouseX = 0;
    mouseY = 0;
}

void Game::update()
{
    float dx = mouseX - player.pos.x;
    float dy = mouseY - player.pos.y;

    player.angle = atan2(dy,dx) * 180.0f / 3.14159265f;
}

void Game::draw()
{
    player.draw();
}