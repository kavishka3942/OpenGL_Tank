#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/freeglut.h>
#include "../include/Game.h"

// -------------------- INPUT STATE --------------------
bool keys[256] = { false };

// -------------------- GAME OBJECT --------------------
//Game game;

// -------------------- DISPLAY --------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    game.update();
    game.draw();

    glutSwapBuffers();
}

// -------------------- MOUSE --------------------
void passiveMotion(int x, int y)
{
    game.mouseX = x;
    game.mouseY = y;
}

// -------------------- KEY DOWN --------------------
void keyDown(unsigned char key, int x, int y)
{
    keys[key] = true;
}

// -------------------- KEY UP --------------------
void keyUp(unsigned char key, int x, int y)
{
    keys[key] = false;
}

// -------------------- IDLE LOOP --------------------
void idle()
{
    // pass keyboard state into game
    game.setInput(keys);

    glutPostRedisplay();
}

// -------------------- MAIN --------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800, 600);

    glutCreateWindow("Tank Game - Phase 2");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);

    // callbacks
    glutDisplayFunc(display);
    glutPassiveMotionFunc(passiveMotion);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}