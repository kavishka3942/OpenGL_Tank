#pragma once

#include "Tank.h"
#include "Projectile.h"
#include <vector>

class Game
{
public:

    Tank player;

    int mouseX;
    int mouseY;

    bool keys[256];

    std::vector<Projectile> bullets;

    Game();

    void setInput(bool inputKeys[256]);

    void shoot();   // NEW

    void update();
    void draw();
};

extern Game game;