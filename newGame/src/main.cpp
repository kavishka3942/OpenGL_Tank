#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/freeglut.h>

#include "../include/Game.h"

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    game.update();
    game.draw();

    glutSwapBuffers();
}

void passiveMotion(int x,int y)
{
    game.mouseX = x;
    game.mouseY = y;
}

void idle()
{
    glutPostRedisplay();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800,600);

    glutCreateWindow("Tank Game");

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0,800,600,0);

    glutDisplayFunc(display);

    glutPassiveMotionFunc(passiveMotion);

    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}