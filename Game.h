#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "object.h"
#include "TTexture.h"

class Game
{
private:
	float w, h;
	float nearZDist = 25;
	float TrackBegin = 2;
	float TrackEnd = -10;
	int score = 0;
	int highScore = 0;
	std::vector<TrackObject *> objects;

	const float playerSize = 1;
	const float trackWidth = 7;
	float trackX[3] = { -trackWidth,0,trackWidth };

	Player player;
	const float playerV = 42.0f; // player's velocity (m/s)

	float viewDist = 25;
	float viewAngle = 40;

	TTexture texCover, texPause, texGameOver;
	TTexture texSky;
	TTexture texTrack;
	
	// sun
	float sunAngle = 10; // east = 10, west = 170

	void DrawTrack();
	void DrawSun();
	void DrawScoreAndPause();

	enum GameState { COVER, PLAYING, PAUSE, GAMEOVER };
	GameState state = PLAYING;
	void Init();
	void displayCover();
	void displayPlay();
	void displayGameOver();

	void SetProjective();
public:
	Game();

	void Update();

	void OnKeyPress(unsigned char key, int x, int y);
	void OnSpecialKeyPress(int key, int x, int y);

	void display();
};

