#pragma once

#include "Tank.h"

class Game
{
public:

    Tank player;

    int mouseX;
    int mouseY;

    Game();

    void update();
    void draw();
};

extern Game game;