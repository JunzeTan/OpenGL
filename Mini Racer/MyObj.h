#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct IndexData
{
	int vertexIndex;
	int texCoordIndex;
	int normalIndex;
};

struct Mesh
{
	std::vector<IndexData> data;
};

class MyObj
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<Mesh> meshes;
public:
	MyObj(std::string filename);

	void Draw() const;
};

std::vector<std::string> Split(const std::string &line, char delim = ' ');
