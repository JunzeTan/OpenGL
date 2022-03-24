// OpenGL and GLUT imports
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#ifndef __PARTICLE__
#define __PARTICLE__

// Particles per object (tweak for performance vs. aesthetic)
static const int NUM_PARTICLES = 100;

// Struct for particles during collision
struct Particle
{
	// Struct fields
	float positionX;
	float positionY;
	float positionZ;
	float velocityX;
	float velocityY;
	float velocityZ;
	float color[3];
	int exists;

	// Constructor to initialize all but position
	Particle()
	{
		this->velocityX = (float)rand() / (float)RAND_MAX * 10 - 5;
		this->velocityY = (float)rand() / (float)RAND_MAX * 10 - 5;
		this->velocityZ = (float)rand() / (float)RAND_MAX * 10 - 5;
		this->exists = 0;
	}

	// Initially position particles close to object
	void position(float x, float y, float z)
	{
		this->exists = 1;
		this->positionX = x;
		this->positionY = y;
		this->positionZ = z;
	}

	// Render particles
	void render(float *c)
	{
		// Display as small circle
		glColor3fv(c);
		glPushMatrix();
		glTranslatef(this->positionX,
			this->positionY, this->positionZ - 50);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTetrahedron();
		glPopMatrix();

		// Move particle in specified direction
		this->positionX += this->velocityX;
		this->positionY += this->velocityY;
		this->positionZ += this->velocityZ;
	}
};

#endif
