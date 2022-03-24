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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

#include "TTexture.h"

using namespace std;

TTexture::TTexture(std::string in_filename):filename(in_filename)
{
	glGenTextures(1, &textureId);

#ifdef _DEBUG
	int err = glGetError();
	assert(err == GL_NO_ERROR);
#endif

	glBindTexture(GL_TEXTURE_2D, textureId);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = nullptr;

	if (GetExt(in_filename) == ".ppm")
	{
		nrChannels = 3;
		int max = 0;
		data = LoadPPM(in_filename.c_str(), &width, &height, &max);
	}
	else
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(in_filename.c_str(), &width, &height, &nrChannels, 0);
	}
	if (data)
	{
		int format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load texture:"+ in_filename);
	}
	stbi_image_free(data);
}

TTexture::TTexture(unsigned int textureId):textureId(textureId)
{
	
}

TTexture::TTexture(const TTexture& other):TTexture(other.filename)
{
}

TTexture::~TTexture()
{
	glDeleteTextures(1, &textureId);
}

void TTexture::Bind(int samplerIndex) const
{
	glBindTexture(GL_TEXTURE_2D, textureId);

#ifdef _DEBUG
	int err = glGetError();
	assert(err == GL_NO_ERROR);
#endif
}


// Read PPM files (Week 9 lecture)
unsigned char *LoadPPM(const char *file, int *width, int *height, int *max)
{
	GLubyte *img;
	FILE *fd;
	int n, m;
	int k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;

	// First open file and check if it's an ASCII PPM
	// (indicated by P3 at the start)
	fd = fopen(file, "r");
	fscanf(fd, "%[^\n] ", b);
	if (b[0] != 'P' || b[1] != '3')
	{
		printf("%s is not a PPM file!\n", file);
		exit(0);
	}
	fscanf(fd, "%c", &c);

	// Next, skip past the comments
	while (c == '#')
	{
		fscanf(fd, "%[^\n] ", b);
		fscanf(fd, "%c", &c);
	}
	ungetc(c, fd);

	// Now get the dimensions and max color value from the image
	fscanf(fd, "%d %d %d", &n, &m, &k);

	// Calculate number of pixels and allocate storage for this
	nm = n * m;
	img = (GLubyte *)malloc(3 * sizeof(GLuint) * nm);
	s = 255.0 / k;

	// For every pixel, grab the read green and blue values,
	// storing them in the image data array
	for (i = 0; i < nm; i++)
	{
		fscanf(fd, "%d %d %d", &red, &green, &blue);
		img[3 * nm - 3 * i - 3] = red * s;
		img[3 * nm - 3 * i - 2] = green * s;
		img[3 * nm - 3 * i - 1] = blue * s;
	}

	// Set the return parameters and return the image array
	*width = n;
	*height = m;
	*max = k;

	return img;
}

std::string GetExt(std::string fileName)
{
	auto pos = fileName.rfind('.');
	if (pos == std::string::npos)
	{
		return "";
	}
	string ans;
	ans = fileName.substr(pos);
	for (char &c : ans)
	{
		c = tolower(c);
	}
	return ans;
}
