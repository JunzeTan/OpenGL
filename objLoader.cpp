// Standard C++ library imports
#include <stdio.h>
#include <stdlib.h>

// OpenGL and GLUT imports
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

// Import header file
#include "objLoader.h"

// Standard input-output stream
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Read object file
objLoader::objLoader(const char* filepath):filepath(filepath)
{
	// Initialize vertices and faces vectors
	vertices = new vector<vector<float>*>;
	faces = new vector<vector<int>*>;

	// Open .obj file
	fstream *objFile = new fstream;
	objFile->open(filepath, ios_base::in);

	// Exit with failure status if can't open file
	if (!objFile->is_open())
	{
		std::cout << "Not able to open the file." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Read until the end of the file
	string line;
	while (!objFile->eof())
	{
		// Read line by line
		std::getline(*objFile, line);

		// Split line by space
		vector<string> *param = this -> splitStr(line, ' ');

		// Prefix is 'v' for vertices
		if (param->at(0) == "v")
		{
			// Eliminate prefix and keep values
			param->erase(param->begin());

			// Convert x, y, z coordinates from str to float
			vector<float> *temp = new vector<float>;
			for (int i = 0; i < param->size(); i++)
			{
				temp->push_back(this->convertStr(param->at(i)));
			}

			// Append vertices
			vertices->push_back(temp);
		}

		// Prefix is 'f' for faces
		else if (param->at(0) == "f")
		{
			// Eliminate prefix and keep values
			param->erase(param->begin());

			// Convert index from str to int
			vector<int> *vertexIndex = new vector<int>;
			for (int i = 0; i < param->size(); i++)
			{
				int faceIndex = this->convertStr(param->at(i));
				vertexIndex->push_back(--faceIndex);
			}

			// Finally save faces information
			faces->push_back(vertexIndex);
		}

		// Delete temp param vector
		delete param;
	}

	// Close .obj file
	objFile->close();
}

// Clean-up of memory
objLoader::~objLoader()
{
	delete this->vertices;
	delete this->faces;
}

// Drawing object
void objLoader::drawObj()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faces->size(); i++) // Each object face
	{
		float coord1[3], coord2[3], coord3[3];
		float *normal = NULL;

		for (int j = 0; j < (faces->at(i))->size(); j++) // Each vertex
		{
			int ind = (faces->at(i))->at(j);
			switch(j)
			{
				case 0: // 1st vertex
					coord1[0] = (vertices->at(ind))->at(0);
					coord1[1] = (vertices->at(ind))->at(1);
					coord1[2] = (vertices->at(ind))->at(2);
					break;
				case 1: // 2nd vertex
					coord2[0] = (vertices->at(ind))->at(0);
					coord2[1] = (vertices->at(ind))->at(1);
					coord2[2] = (vertices->at(ind))->at(2);
					break;
				case 2: // 3rd vertex
					coord3[0] = (vertices->at(ind))->at(0);
					coord3[1] = (vertices->at(ind))->at(1);
					coord3[2] = (vertices->at(ind))->at(2);
					break;
			}

			// Calculate norm and draw triangles since we have 3 vertices
			if (j == 2)
			{
				normal = this->getNorm(coord1, coord2, coord3);
				glNormal3f(normal[0], normal[1], normal[2]);

				glVertex3f(coord1[0], coord1[1], coord1[2]);
				glVertex3f(coord2[0], coord2[1], coord2[2]);
				glVertex3f(coord3[0], coord3[1], coord3[2]);
			}
		}
	}
	glEnd();
}

// Split string by given separator
vector<string>* objLoader::splitStr(string sentence, char separator)
{
	sentence += separator;

	string word = "";
	vector<string> *words = new vector<string>;

	for (int ch = 0; ch< sentence.length(); ch++)
	{
		if (sentence[ch] != separator)
		{
			word += sentence[ch];
		}
		else
		{
			words->push_back(word);
			word = "";
		}
	}
	return words;
}

// Converting string to float
float objLoader::convertStr(string sentence)
{
	float val = 0.0f;
	istringstream buffer(sentence);
	buffer >> val;
	return val;
}

// Calculating norm
float* objLoader::getNorm(float *coord1, float *coord2, float *coord3)
{
	// Declare two vectors and cross product
	float v1[3], v2[3], cross[3];

	// Calculate first vector
	v1[0] = coord1[0] - coord2[0];
	v1[1] = coord1[1] - coord2[1];
	v1[2] = coord1[2] - coord2[2];

	// Calculate second vector
	v2[0] = coord1[0] - coord3[0];
	v2[1] = coord1[1] - coord3[1];
	v2[2] = coord1[2] - coord3[2];

	// Calculate cross product
	cross[0] = v1[1] * v2[2] - v2[1] * v1[2];
	cross[1] = v2[0] * v1[2] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v2[0] * v1[1];

	// Calculate normalization factor
	float fact = sqrt(pow(cross[0], 2) + pow(cross[1], 2)
		+ pow(cross[2], 2));

	// Normalize vectors (to be returned)
	static float norm[3];
	norm[0] = cross[0] / fact;
	norm[1] = cross[1] / fact;
	norm[2] = cross[2] / fact;
	return norm;
}
