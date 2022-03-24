#include "MyObj.h"

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

#include <string>
#include <vector>
#include <fstream>
#include <assert.h>

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

enum LineType { UNKNOWN, COMMENT, VERTEX, NORMAL,TEXCOORD, MTLLIB, USEMTL, OBJECT, GROUP, FACE };

std::vector<std::string> Split(const std::string &line, char delim)
{
	string::size_type i = 0;
	string::size_type len = line.length();
	vector<string> ans;

	while (i < len)
	{
		if (line[i] != delim)
		{
			break;
		}
		i++;
	}
	int start = i;

	while (i < len)
	{
		if (line[i] == delim)
		{
			int end = i;
			if (line[start] != delim)
				ans.push_back(line.substr(start, end - start));
			start = i + 1;
		}
		i++;
	}

	if (start < len)
		ans.push_back(line.substr(start, len - start));
	return ans;
}

void TestSplit()
{
	auto ans = Split("");
	assert(ans.size() == 0);

	ans = Split(" ");
	assert(ans.size() == 0);

	ans = Split("  ");
	assert(ans.size() == 0);

	ans = Split(" a ");
	assert(ans.size() == 1);

	ans = Split("  a  ");
	assert(ans.size() == 1);

	ans = Split("  a ");
	assert(ans.size() == 1);

	ans = Split("a  b");
	assert(ans.size() == 2);

	ans = Split("  a  b   ");
	assert(ans.size() == 2);

	ans = Split("   a   b   c  ");
	assert(ans.size() == 3);

	ans = Split("a b c");
	assert(ans.size() == 3);
}

/*

def process_line(line):
	"""
	Function for reading the Blender3D object file, line by line. Clearly
	minimalistic and slow as it is, but it will do the job nicely for this course.
	"""
	label = None
	fields = line.split()

	elif fields[0] == 'f':
		label = 'face'
		if len(fields) != 4 and len(fields) != 5:
			print('(E) Error, 3 or 4 entries expected for faces\n{}'.format(line))
			return None

		for vtn_str in fields[1:]:  # vtn_str='586/1' , '1860/2'
			vtn = vtn_str.split(('/'))
			if len(vtn) == 1:  # vtn_str=
				indices.append(np.uint32(vtn[0]) - 1)
			elif len(vtn) == 2:  # vtn_str=
				indices.append(np.uint32(vtn[0]) - 1)
				textures.append(np.uint32(vtn[1]) - 1)
			elif len(vtn) == 3:  # vtn_str=
				indices.append(np.uint32(vtn[0]) - 1)
				if len(vtn[1]) == 0:
					pass  # do nothing
				else:
					textures.append(np.uint32(vtn[1]) - 1)
				normals.append(np.uint32(vtn[2]) - 1)

		# print([ [np.uint32(i)-1 for i in v.split('/')] for v in fields[1:] ])
		return (label, indices, normals, textures)
	# return ( label, [ [np.uint32(i)-1 for i in v.split('/')] for v in fields[1:] ] )

	else:
		print('(E) Unknown line: {}'.format(fields))
		return None

	return (label, [float(token) for token in fields[1:]])

*/

MyObj::MyObj(std::string filename)
{
	TestSplit();
	ifstream ifs(filename);
	assert(ifs.is_open());

	int faceNum = 0;
	Mesh tempMesh;
	string curObjectName;
	string line;
	LineType type;
	while (getline(ifs, line))
	{
		if (line.empty())
		{
			continue;
		}

		auto fields = Split(line);
		if (fields[0] == "#")
		{
			type = COMMENT;
			continue;
		}

		if (fields[0] == "v")
		{
			type = VERTEX;
			assert(fields.size() - 1 == 3);

			vec3 data(stof(fields[1]), stof(fields[2]), stof(fields[3]));
			vertices.push_back(data);
			continue;
		}

		if (fields[0] == "vn")
		{
			type = NORMAL;
			assert(fields.size() - 1 == 3);

			vec3 data(stof(fields[1]), stof(fields[2]), stof(fields[3]));
			normals.push_back(data);
			continue;
		}

		if (fields[0] == "vt")
		{
			type = TEXCOORD;
			assert(fields.size() - 1 == 2 || fields.size()-1 == 3);

			vec2 data(stof(fields[1]), stof(fields[2]));
			texCoords.push_back(data);
			continue;
		}

		if (fields[0] == "mtllib")
		{
			type = MTLLIB;
			assert(fields.size() - 1 == 1);

			string matName = fields[1];
			continue;
		}

		if (fields[0] == "usemtl")
		{
			type = USEMTL;
			assert(fields.size() - 1 == 1);

			string matName = fields[1];
			continue;
		}

		if (fields[0] == "o")
		{
			type = OBJECT;
			assert(fields.size() - 1 == 1);

			if (faceNum)
			{
				meshes.push_back(tempMesh);

				// clear
				faceNum = 0;
				tempMesh.data.clear();
			}
			string objName = fields[1];
			curObjectName = objName;
			continue;
		}

		if (fields[0] == "g")
		{
			type = GROUP;
			assert(fields.size() - 1 == 1);

			string groupName = fields[1];
			continue;
		}

		if (fields[0] == "f")
		{
			type = FACE;
			assert(fields.size() - 1 == 3);

			//# multiple formats for faces lines, eg
			//# f 586/1 1860/2 1781/3
			//# f vi/ti/ni
			//# where vi is the vertex index
			//# ti is the texture index
			//# ni is the normal index (optional)
			//# note that indexing in the file starts at 1, so we need to correct to start at zero
			//indices = []
			//textures = []
			//normals = []
			ivec3 indices;
			ivec3 texCoordIndices;
			ivec3 normalIndices;

			for (int i = 1; i < fields.size(); ++i)
			{
				vector<string> vtn = Split(fields[i], '/');
				if (vtn.size() == 1) //'586'格式
				{
					indices[i - 1] = stoi(vtn[0]) - 1;
					continue;
				}
				if (vtn.size() == 2) //'586/1'格式
				{
					indices[i - 1] = stoi(vtn[0]) - 1;
					texCoordIndices[i - 1] = stoi(vtn[1]) - 1;
					continue;
				}
				if (vtn.size() == 3) //'586/2/1' 格式，或者'586//1'格式
				{
					indices[i - 1] = stoi(vtn[0]) - 1;
					texCoordIndices[i - 1] = stoi(vtn[1]) - 1;
					normalIndices[i - 1] = stoi(vtn[2]) - 1;
					continue;
				}
			}

			for (int i = 0; i < 3; ++i)
			{
				IndexData indexData;
				indexData.vertexIndex = indices[i];
				indexData.normalIndex = normalIndices[i];
				indexData.texCoordIndex = texCoordIndices[i];
				tempMesh.data.push_back(indexData);
			}
			faceNum++;
			continue;
		}
	}

	if (faceNum)
	{
		meshes.push_back(tempMesh);
	}
}

void MyObj::Draw() const
{
	glBegin(GL_TRIANGLES);
	for (auto &mesh : meshes)
	{
		for (auto &indexData : mesh.data)
		{
			glNormal3f(normals[indexData.normalIndex].x, normals[indexData.normalIndex].y, normals[indexData.normalIndex].z);
			glTexCoord2f(texCoords[indexData.texCoordIndex].x, texCoords[indexData.texCoordIndex].y);
			glVertex3f(vertices[indexData.vertexIndex].x, vertices[indexData.vertexIndex].y, vertices[indexData.vertexIndex].z);
		}
	}
	glEnd();
}
