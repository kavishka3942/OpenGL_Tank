#include "../include/Game.h"
#include <cmath>
#include <algorithm>
#include <GL/freeglut.h>
#include <cstdlib>
#include <cstdio>
#include <vector>

namespace
{
    void fillPolygonAt(const std::vector<Vector2D> &localVertices, float offsetX, float offsetY, float angleDegrees)
    {
        if(localVertices.size() < 3) return;

        std::vector<Vector2D> vertices;
        vertices.reserve(localVertices.size());

        float radians = angleDegrees * 3.14159265f / 180.0f;
        float cosAngle = std::cos(radians);
        float sinAngle = std::sin(radians);

        for(const Vector2D &vertex : localVertices)
        {
            float rotatedX = vertex.x * cosAngle - vertex.y * sinAngle;
            float rotatedY = vertex.x * sinAngle + vertex.y * cosAngle;
            vertices.push_back(Vector2D(rotatedX + offsetX, rotatedY + offsetY));
        }

        float minY = vertices[0].y;
        float maxY = vertices[0].y;
        for(const Vector2D &vertex : vertices)
        {
            if(vertex.y < minY) minY = vertex.y;
            if(vertex.y > maxY) maxY = vertex.y;
        }

        glBegin(GL_POINTS);

        int startY = static_cast<int>(std::ceil(minY));
        int endY = static_cast<int>(std::floor(maxY));

        for(int y = startY; y <= endY; y++)
        {
            float scanY = static_cast<float>(y);
            std::vector<float> intersections;

            for(size_t i = 0; i < vertices.size(); i++)
            {
                const Vector2D &current = vertices[i];
                const Vector2D &next = vertices[(i + 1) % vertices.size()];

                if((current.y > scanY) != (next.y > scanY))
                {
                    float x = current.x + (scanY - current.y) * (next.x - current.x) / (next.y - current.y);
                    intersections.push_back(x);
                }
            }

            std::sort(intersections.begin(), intersections.end());

            for(size_t i = 0; i + 1 < intersections.size(); i += 2)
            {
                int startX = static_cast<int>(std::ceil(intersections[i]));
                int endX = static_cast<int>(std::floor(intersections[i + 1]));

                for(int x = startX; x <= endX; x++)
                    glVertex2f(static_cast<float>(x), scanY);
            }
        }

        glEnd();
    }
}

Game game;

Game::Game()
{
    mouseX = 0;
    mouseY = 0;

    camX = 0;
    camY = 0;

    playerHP = 10;
    gameOver = false;
    score = 0;

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
    score = 0;

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
                 score += 1;   // ADD SCORE

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

void drawText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    while(*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
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

    char buffer[50];

    // HP TEXT
    sprintf(buffer, "HP: %d", playerHP);
    drawText(20, 20, buffer);

    // SCORE TEXT
    sprintf(buffer, "Score: %d", score);
    drawText(20, 50, buffer);

    // BACKGROUND BAR (gray)
    glColor3f(0.3f, 0.3f, 0.3f);
    fillPolygonAt({
        Vector2D(20, 70),
        Vector2D(220, 70),
        Vector2D(220, 90),
        Vector2D(20, 90)
    }, 0.0f, 0.0f, 0.0f);

    float hpPercent = playerHP / 10.0f;   // assuming max HP = 10
    float barWidth = 200 * hpPercent;
    
    // color transition (green → red)
    glColor3f(1.0f - hpPercent, hpPercent, 0.0f);
    fillPolygonAt({
        Vector2D(20, 70),
        Vector2D(20 + barWidth, 70),
        Vector2D(20 + barWidth, 90),
        Vector2D(20, 90)
    }, 0.0f, 0.0f, 0.0f);

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