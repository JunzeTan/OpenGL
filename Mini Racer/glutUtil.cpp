#include "glutUtil.h"

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

void glutDrawText(float x, float y, const char *string)
{
	glRasterPos2f(x, y);
	int len = strlen(string);
	for (int i = 0; i < len; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}
