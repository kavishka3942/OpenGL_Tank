#pragma once

#include "Tank.h"
#include "Projectile.h"
#include "Obstacle.h"
#include "EnemyTank.h"
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

    int playerHP;
    bool gameOver;

    int maxEnemies;

    std::vector<Projectile> bullets;
    std::vector<Projectile> enemyBullets;
    std::vector<Obstacle> obstacles;
    std::vector<EnemyTank> enemies;

    Game();

    void setInput(bool inputKeys[256]);

    void shoot();

    void enemyShoot(EnemyTank &e);

    void spawnEnemy();

    void restart();

    void update();
    void draw();
};

extern Game game;