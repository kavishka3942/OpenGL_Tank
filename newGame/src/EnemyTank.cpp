#include "../include/EnemyTank.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <GL/freeglut.h>

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

EnemyTank::EnemyTank(float x, float y)
{
    pos = Vector2D(x, y);
    angle = 0;
    alive = true;
    shootTimer = 0;
}

void EnemyTank::update(Vector2D playerPos)
{
    float dx = playerPos.x - pos.x;
    float dy = playerPos.y - pos.y;

    angle = atan2(dy, dx) * 180.0f / 3.14159265f;

    shootTimer += 0.016f;
}

void EnemyTank::draw()
{
    if(!alive) return;

    glColor3f(1,0,0);

    std::vector<Vector2D> body = {
        Vector2D(-20, -15),
        Vector2D(20, -15),
        Vector2D(20, 15),
        Vector2D(-20, 15)
    };

    fillPolygonAt(body, pos.x, pos.y, angle);

    glColor3f(0.3f,0.3f,0.3f);

    std::vector<Vector2D> turret = {
        Vector2D(0, -3),
        Vector2D(30, -3),
        Vector2D(30, 3),
        Vector2D(0, 3)
    };

    fillPolygonAt(turret, pos.x, pos.y, angle);
}

bool EnemyTank::checkHit(Vector2D bulletPos)
{
    float dx = bulletPos.x - pos.x;
    float dy = bulletPos.y - pos.y;

    return (sqrt(dx*dx + dy*dy) < 25);
}