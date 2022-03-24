#include <stdio.h>

#ifdef APPLE
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#include <Windows.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#define FREEGLUT_STATIC
#  include <GL/freeglut.h>
#endif

#include "object.h"

Object::Object(float x, float y, float z) :pos(x, y, z), type(NONE)
{
}

void Object::Render(float r, float g, float b, float s)
{
	glPushMatrix();
	glColor3f(r, g, b);

	GLfloat diffuse[] = { r,g,b,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidCube(3);
	glPopMatrix();
}

Player::Player() : TrackObject(1, 0)
{
	this->type = CHARACTER;
	pos.x = GetX();
	pos.y = 1.0f;
}

void Player::Render()
{
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	ResourceManager::texCar->Bind();

	glColor3f(1, 1, 1);
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(180, 0, 1, 0);
	glScalef(0.8, 0.8, 0.8);

	GLfloat diffuse[] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	ResourceManager::car->Draw();

	//glutSolidSphere(2, 10, 10);
	glPopMatrix();
}

void Player::MoveLeft()
{
	if (trackIndex > 0)
		trackIndex--;
	pos.x = GetX();
}

void Player::MoveRight()
{
	if (trackIndex < 3 - 1)
		trackIndex++;
	pos.x = GetX();
}

Building::Building(int trackIndex, float y, float z) : TrackObject(trackIndex, 1, y, z) // building's value = 1
{
	this->type = BUILDING;
	pos.y = 1.0f;

	angle = rand() % 360;
}

void Building::Render()
{
	//Object::Render(0.139, 0.69, 0.19, 1); // green

	glPushMatrix();
	//glColor3f(0.8, 0.1, 0.19);
	glEnable(GL_TEXTURE_2D);
	ResourceManager::texStone->Bind();

	//GLfloat diffuse[] = { 0.8, 0.1, 0.19,1 }; // red
	GLfloat diffuse[] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(angle, 0, 1, 0);
	//glutSolidTeapot(3);

	ResourceManager::stone->drawObj();

	glPopMatrix();
}

Coin::Coin(int trackIndex, float y, float z) : TrackObject(trackIndex, 2, y, z), // coin's value = 2
angle(0)
{
	type = COIN;
	pos.y = 1.0;
}

void Coin::Render()
{
	glPushMatrix();
	//glColor3f(0.8, 0.1, 0.19);
	glEnable(GL_TEXTURE_2D);
	ResourceManager::texCoin->Bind();

	//GLfloat diffuse[] = { 0.8, 0.1, 0.19,1 }; // red
	GLfloat diffuse[] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(angle, 0, 1, 0);
	//glutSolidTeapot(3);

	ResourceManager::coin->drawObj();

	glPopMatrix();

	angle += 10.0f;
}

float TrackObject::trackWidth = 5;

void TrackObject::SetTrackWidth(float trackWidth_)
{
	trackWidth = trackWidth_;
}

MyObj *ResourceManager::car = nullptr;
TTexture *ResourceManager::texCar = nullptr;
objLoader *ResourceManager::stone = nullptr;
TTexture *ResourceManager::texStone = nullptr;
objLoader *ResourceManager::coin = nullptr;
TTexture *ResourceManager::texCoin = nullptr;

void ResourceManager::LoadResource()
{
	if (car == nullptr)
		car = new MyObj("Objects/car.obj");
	if (texCar == nullptr)
		texCar = new TTexture("Textures/car.png");

	if (stone == nullptr)
		stone = new objLoader("Objects/rock.obj");
	if (texStone == nullptr)
		texStone = new TTexture("Textures/rock.ppm");

	if (coin == nullptr)
		coin = new objLoader("Objects/coin.obj");
	if (texCoin == nullptr)
		texCoin = new TTexture("Textures/coin.ppm");
}
