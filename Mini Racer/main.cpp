
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#include <windows.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#define FREEGLUT_STATIC
#  include <GL/freeglut.h>
#endif

#include "Game.h"

int Width = 600;
int Height = 600;

Game *game;

void keyboard(unsigned char key, int x, int y)
{
    game->OnKeyPress(key, x, y);
}

void OnSpecialKey(int key, int x, int y)
{
    game->OnSpecialKeyPress(key, x, y);
}

void display()
{
    game->display();
}

void refresh(int)
{
    glutTimerFunc(1000 / 60, refresh, 0);
    glutPostRedisplay();
}

void WindowReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w/(double)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    int offsetWidth = ((glutGet(GLUT_SCREEN_WIDTH) - Width) / 2);
	int offsetHeight = ((glutGet(GLUT_SCREEN_HEIGHT) - Height) / 2);
    glutInitWindowSize(Width, Height);
	glutInitWindowPosition(offsetWidth,offsetHeight);
    glutCreateWindow("Racer");

    game = new Game;
    glClearColor(0.1, 0.1, 0.1, 1);
    WindowReshape(Width, Height);

    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(WindowReshape);
	glutSpecialFunc(OnSpecialKey);
    glutTimerFunc(1000 / 60, refresh, 0);
    //glutIdleFunc(refresh);
    glutMainLoop();
    delete game;
    return 0;
}






