#include "Game.h"

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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#undef max

#include <iostream>
#include <string>

#include "glutUtil.h"

using namespace std;
using namespace glm;


Game::Game() : texCover("Textures/cover.png"), texPause("Textures/pause.png"), texGameOver("Textures/gameover.png"),
texSky("Textures/sky.jpg"),
texTrack("Textures/track.png"),
w(600), h(600)
{
	Init();
}

void Game::Init()
{
	score = 0;
	player.pos = vec3(0);
	for (auto &obj : objects)
	{
		delete obj;
	}
	objects.clear();

	// sun
	sunAngle = 0;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ResourceManager::LoadResource();
}

void Game::displayCover()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	{
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 600, 0, 600, -1, 1); // screen pixel zone

		// draw
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		texCover.Bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, 600);
		glTexCoord2f(1, 1); glVertex2f(600, 600);
		glTexCoord2f(1, 0); glVertex2f(600, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	glFlush();
}

void Game::displayPlay()
{
	static float prevTime = glutGet(GLUT_ELAPSED_TIME);
	float nowTime = glutGet(GLUT_ELAPSED_TIME);
	float dt = nowTime - prevTime; // unit = ms
	//cout << dt << endl;

	if (state == PLAYING)
	{
		// update player's pos
		player.pos.z -= playerV * dt * 0.001f;

		Update();
	}

	// start render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// render background sky
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		{
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, 600, 0, 600, -1, 1); // screen pixel zone

			// draw
			glEnable(GL_TEXTURE_2D);
			texSky.Bind();
			glColor4f(1, 1, 1, 1);

			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(0, 0);
			glTexCoord2f(0, 1); glVertex2f(0, 600);
			glTexCoord2f(1, 1); glVertex2f(600, 600);
			glTexCoord2f(1, 0); glVertex2f(600, 0);
			glEnd();

			glPopMatrix();
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vec3 eye = { 0,viewDist * sin(glm::radians(viewAngle)),viewDist * cos(glm::radians(viewAngle)) + player.pos.z };
	vec3 target = { 0,0,player.pos.z - 10 };
	gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, 0, 1, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glColor4f(1, 1, 1, 1);
	DrawTrack();

	for (auto obj : objects)
	{
		obj->Render();
		//cout << obj->pos.x << endl;
	}

	player.Render();

	DrawSun();

	// object rendering end
	DrawScoreAndPause();

	glFlush();

	// end of rendering

	prevTime = nowTime;

	//system("pause");
}

void Game::displayGameOver()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	{
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 600, 0, 600, -1, 1); // screen pixel zone

		// draw
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		texGameOver.Bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(0, 0);
		glTexCoord2f(0, 1); glVertex2f(0, 600);
		glTexCoord2f(1, 1); glVertex2f(600, 600);
		glTexCoord2f(1, 0); glVertex2f(600, 0);
		glEnd();

		// draw score
		glColor3f(1, 0, 0);

		string s = "score: " + to_string(score);
		glutDrawText(10, 570, s.c_str());

		s = "high score: " + to_string(highScore);
		glutDrawText(450, 570, s.c_str());

		glPopMatrix();
	}

	glFlush();
}

void Game::SetProjective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / (double)h, 0.1, 1000.0);
}

void Game::DrawTrack()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float farZ = player.pos.z - 150;
	float nearZ = player.pos.z + 20;

	float totalWidth = trackWidth * 3 + trackWidth * 0.5 * 2;

	GLfloat diffuse[] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	//glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);
	texTrack.Bind();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0); glTexCoord2f(0, 0); glVertex3f(-totalWidth / 2.0f, 0, nearZ);
	glNormal3f(0, 1, 0); glTexCoord2f(1, 0); glVertex3f(+totalWidth / 2.0f, 0, nearZ);
	glNormal3f(0, 1, 0); glTexCoord2f(1, 1); glVertex3f(+totalWidth / 2.0f, 0, farZ);
	glNormal3f(0, 1, 0); glTexCoord2f(0, 1); glVertex3f(-totalWidth / 2.0f, 0, farZ);
	glEnd();
}

void Game::DrawSun()
{
	// 
	float sunDist = 10;
	vec4 sunDir = { 1,0,0,0 };
	mat4 R(1.0f);
	R = glm::rotate(R, radians(sunAngle), vec3(0, 0, 1));
	sunDir = R * sunDir;

	vec4 sunPos = vec4(0, 0, player.pos.z - 5, 1) + sunDist * sunDir;

	glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(sunPos));

	vec4 lightAmbient = { vec3(0.5),1 };
	vec4 lightDiffuse = { 1,1,1,1 };
	vec4 lightSpecular = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(lightAmbient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(lightDiffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(lightSpecular));

	GLfloat sunDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sunDiffuse);

	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(0.8, 0.1, 0.19);
	glTranslatef(sunPos.x, sunPos.y, sunPos.z);
	glutSolidSphere(0.5, 16, 16);
	glPopMatrix();
	glEnable(GL_LIGHTING);

	if (state == PLAYING)
	{
		sunAngle += 0.1;
		if (sunAngle >= 170)
			sunAngle = 10;
	}
}

void Game::DrawScoreAndPause()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity(); // make model matrix = mat(1)

	glMatrixMode(GL_PROJECTION);
	{
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 600, 0, 600, -1, 1); // screen pixel zone

		// draw pause
		if (state == PAUSE)
		{
			glEnable(GL_TEXTURE_2D);
			glColor4f(1, 1, 1, 1);
			texPause.Bind();
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(0, 0);
			glTexCoord2f(0, 1); glVertex2f(0, 600);
			glTexCoord2f(1, 1); glVertex2f(600, 600);
			glTexCoord2f(1, 0); glVertex2f(600, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}

		// draw score
		glColor3f(1, 0, 0);

		string s = "score: " + to_string(score);
		glutDrawText(10, 570, s.c_str());

		if (state == PAUSE)
		{
			s = "high score: " + to_string(highScore);
			glutDrawText(450, 570, s.c_str());
		}

		glPopMatrix();
	}
}

void Game::Update()
{
	if (!objects.empty()) {
		while (objects[0]->pos.z > player.pos.z + nearZDist) // out of the near line
		{
			delete objects[0];
			objects.erase(objects.begin());
		}
	}

	for (auto it = objects.begin(); it != objects.end();)
	{
		auto obj = *it;
		switch (obj->type)
		{
		case BUILDING:
		{
			if (player.pos.z - playerSize < obj->pos.z && obj->pos.z < player.pos.z + playerSize) // z-collision
			{
				if (player.GetTrackIndex() == obj->GetTrackIndex()) // same x
				{
					// game over
					state = GAMEOVER;
					//cout << "player z=" << player.pos.z << " obj z=" << obj->pos.z << endl;
					//cout << player.GetTrackIndex() << " " << obj->GetTrackIndex() << endl;
					highScore = std::max(highScore, score);
					return;
				}
			}
			if (obj->pos.z > player.pos.z + playerSize) // is out of player
			{
				// get score
				score += obj->value;
				obj->value = 0;
			}
		}
		break;
		case COIN:
		{
			if (player.pos.z - playerSize < obj->pos.z && obj->pos.z < player.pos.z + playerSize) // z-collision
			{
				if (player.GetTrackIndex() == obj->GetTrackIndex()) // same x
				{
					// get score
					score += obj->value;
					obj->value = 0;

					// erase this coin
					delete obj;
					it = objects.erase(it);
					continue;
				}
			}
		}
		break;
		}
		it++;
	}

	// add new
	if (objects.empty() || objects.back()->pos.z - nearZDist > player.pos.z + TrackEnd * nearZDist)
	{
		const float probilityCoin = 0.2;
		const float probilityBuilding = 0.2;

		float y = 0;
		float z = player.pos.z + TrackEnd * nearZDist; // create at the end of horizon

		vector<int> buildingTrackIndex;
		vector<int> coinTrackIndex;

		// for each empty position
		for (int trackIndex = 0; trackIndex < 3; ++trackIndex) // 
		{
			int rd = rand() % 11; // [0,10]
			float rdFloat = (float)rd / 10.0f; // [0,1.0]
			if (rdFloat > probilityCoin + probilityBuilding)
			{
				// nothing
			}
			else
			{
				if (rdFloat >= probilityCoin) // [coin, building] -> create building
				{
					buildingTrackIndex.push_back(trackIndex);
				}
				else
				{
					coinTrackIndex.push_back(trackIndex);
				}
			}
		}

		// if all position is building, erase one
		if (buildingTrackIndex.size() == 3)
		{
			int eraseIndex = rand() % 3;
			buildingTrackIndex.erase(buildingTrackIndex.begin() + eraseIndex);
		}

		for (int trackIndex : buildingTrackIndex)
		{
			objects.push_back(new Building(trackIndex, y, z));
		}
		for (int trackIndex : coinTrackIndex)
		{
			objects.push_back(new Coin(trackIndex, y, z));
		}
	}
}

void Game::OnKeyPress(unsigned char key, int x, int y)
{
	if (key == 27 || key == 'q' || key == 'Q') {
		exit(0);
	}

	if (state == COVER)
	{
		if (key == 's' || key == 'S')
		{
			state = PLAYING;
			Init();
		}
		return;
	}

	if (state == PLAYING)
	{
		if (key == 'p' || key == 'P')
		{
			state = PAUSE;
		}
		return;
	}

	if (state == PAUSE)
	{
		if (key == 'p' || key == 'P')
		{
			state = PLAYING;
		}
		return;
	}

	if (state == GAMEOVER)
	{
		if (key == 's' || key == 'S')
		{
			state = PLAYING;
			Init();
			return;
		}
	}
}

void Game::OnSpecialKeyPress(int key, int x, int y)
{
	if (state == PLAYING)
	{
		if (key == GLUT_KEY_LEFT)
		{
			player.MoveLeft();
		}
		if (key == GLUT_KEY_RIGHT)
		{
			player.MoveRight();
		}
		//if (key == GLUT_KEY_UP)
		//{
		//	viewAngle++;
		//}
		//if (key == GLUT_KEY_DOWN)
		//{
		//	viewAngle--;
		//}
	}
	glutPostRedisplay();
}

void Game::display()
{
	switch (state)
	{
	case COVER:
		displayCover();
		break;
	case PLAYING:
	case PAUSE:
		displayPlay();
		break;
	case GAMEOVER:
		displayGameOver();
		break;
	}
}
