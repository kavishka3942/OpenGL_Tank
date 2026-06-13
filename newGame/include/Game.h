#pragma once

#include "Tank.h"

class Game
{
public:

    Tank player;

    int mouseX;
    int mouseY;

    bool keys[256];   // ✅ ADD THIS

    Game();

    void setInput(bool inputKeys[256]);

    void update();
    void draw();
};

extern Game game;