#pragma once

#include "Tank.h"
#include "Projectile.h"
#include "Obstacle.h"
#include <vector>

class Game
{
public:

    Tank player;

    int mouseX;
    int mouseY;

    bool keys[256];

    float camX;
    float camY;

    const float WORLD_W = 2000;
    const float WORLD_H = 2000;

    std::vector<Projectile> bullets;
    std::vector<Obstacle> obstacles;

    Game();

    void setInput(bool inputKeys[256]);

    void shoot();

    void update();
    void draw();
};

extern Game game;