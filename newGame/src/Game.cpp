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

    playerHP = 10;
    gameOver = false;

    maxEnemies = 5;

    for(int i = 0; i < 256; i++)
        keys[i] = false;

    // OBSTACLES
    for(int i = 0; i < 80; i++)
    {
        float x = rand() % 1800 + 50;
        float y = rand() % 1800 + 50;
        obstacles.push_back(Obstacle(x, y, 40));
    }

    // INITIAL ENEMIES
    for(int i = 0; i < maxEnemies; i++)
        spawnEnemy();
}

void Game::setInput(bool inputKeys[256])
{
    for(int i = 0; i < 256; i++)
        keys[i] = inputKeys[i];
}

void Game::shoot()
{
    if(gameOver) return;

    bullets.push_back(
        Projectile(player.pos.x, player.pos.y, player.angle, false)
    );
}

void Game::enemyShoot(EnemyTank &e)
{
    if(e.shootTimer > 2.0f)
    {
        float dx = player.pos.x - e.pos.x;
        float dy = player.pos.y - e.pos.y;

        float angle = atan2(dy, dx) * 180.0f / 3.14159265f;

        enemyBullets.push_back(
            Projectile(e.pos.x, e.pos.y, angle, true)
        );

        e.shootTimer = 0;
    }
}

void Game::restart()
{
    playerHP = 10;
    gameOver = false;

    bullets.clear();
    enemyBullets.clear();
    enemies.clear();

    player.pos = Vector2D(100,100);

    for(int i = 0; i < 6; i++)
    {
        float x = rand() % 2000;
        float y = rand() % 2000;
        enemies.push_back(EnemyTank(x,y));
    }
}

void Game::update()
{
    if(gameOver)
    {
        if(keys[13]) restart();
        return;
    }

    const float WORLD_W = 2000;
    const float WORLD_H = 2000;

    float speed = 2.5f;

    float nextX = player.pos.x;
    float nextY = player.pos.y;

    if(keys['w']) nextY -= speed;
    if(keys['s']) nextY += speed;
    if(keys['a']) nextX -= speed;
    if(keys['d']) nextX += speed;

    if(nextX < 20) nextX = 20;
    if(nextX > WORLD_W - 20) nextX = WORLD_W - 20;
    if(nextY < 20) nextY = 20;
    if(nextY > WORLD_H - 20) nextY = WORLD_H - 20;

    bool canMove = true;

    for(auto &o : obstacles)
        if(o.checkCollision(nextX, nextY))
            canMove = false;

    if(canMove)
    {
        player.pos.x = nextX;
        player.pos.y = nextY;
    }

    // CAMERA
    camX = player.pos.x - 400;
    camY = player.pos.y - 300;

    if(camX < 0) camX = 0;
    if(camY < 0) camY = 0;
    if(camX > WORLD_W - 800) camX = WORLD_W - 800;
    if(camY > WORLD_H - 600) camY = WORLD_H - 600;

    // MOUSE AIM
    float wx = mouseX + camX;
    float wy = mouseY + camY;

    float dx = wx - player.pos.x;
    float dy = wy - player.pos.y;

    player.angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    // PLAYER BULLETS
    for(auto &b : bullets)
    {
        b.update();

        for(auto &e : enemies)
            if(e.alive && e.checkHit(b.pos))
                e.alive = false;

        for(auto &o : obstacles)
            if(o.checkCollision(b.pos.x, b.pos.y))
                b.active = false;
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
        [](Projectile &b){ return !b.active; }),
        bullets.end()
    );

    // ENEMY BULLETS
    for(auto &b : enemyBullets)
    {
        b.update();

        float dx = b.pos.x - player.pos.x;
        float dy = b.pos.y - player.pos.y;

        if(sqrt(dx*dx + dy*dy) < 20)
        {
            playerHP--;
            b.active = false;

            if(playerHP <= 0)
                gameOver = true;
        }

        for(auto &o : obstacles)
            if(o.checkCollision(b.pos.x, b.pos.y))
                b.active = false;
    }

    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [](Projectile &b){ return !b.active; }),
        enemyBullets.end()
    );

    // ENEMIES UPDATE + SHOOT
    for(auto &e : enemies)
    {
        if(!e.alive) continue;

        e.update(player.pos);
        enemyShoot(e);
    }

    // 🔥 FIXED RESPAWN SYSTEM (ALWAYS 5 ACTIVE)
    for(int i = 0; i < enemies.size(); i++)
    {
        if(!enemies[i].alive)
        {
            enemies.erase(enemies.begin() + i);
            spawnEnemy();
            i--;
        }
    }
}

void Game::draw()
{
    glClearColor(0.9f,0.8f,0.6f,1);
    glClear(GL_COLOR_BUFFER_BIT);

    const float WORLD_W = 2000;
    const float WORLD_H = 2000;

    glPushMatrix();
    glTranslatef(-camX,-camY,0);

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0,0);
        glVertex2f(WORLD_W,0);
        glVertex2f(WORLD_W,WORLD_H);
        glVertex2f(0,WORLD_H);
    glEnd();

    for(auto &o : obstacles) o.draw();
    for(auto &b : bullets) b.draw();
    for(auto &b : enemyBullets) b.draw();
    for(auto &e : enemies) e.draw();

    player.draw();

    glPopMatrix();

    if(gameOver)
    {
        glColor3f(1,0,0);
        glRasterPos2f(350,300);
        const char *msg = "GAME OVER - PRESS ENTER";
        while(*msg) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*msg++);
    }
}
void Game::spawnEnemy()
{
    float x = rand() % 2000;
    float y = rand() % 2000;

    // avoid initial camera zone
    if(x < 800 && y < 600)
        x += 900;

    enemies.push_back(EnemyTank(x, y));
}