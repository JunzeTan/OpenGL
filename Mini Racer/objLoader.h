#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

class objLoader{
public:
	objLoader(const char* filepath); // Filepath of .obj file
	~objLoader(); // Destructor
	void drawObj(); // Drawing

private:
	const char* filepath;

	vector<vector<float>*> *vertices;
	vector<vector<int>*> *faces;

	// Split string given by a separator character
	vector<string>* splitStr(string sentence, char separator);

	// Converting string value to float
	float convertStr(string sentence);

	// Calculate normal
	float* getNorm(float *coord1, float *coord2, float *coord3);
};

#endif
