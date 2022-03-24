#pragma once

#include <string>

class TTexture
{
private:
	std::string filename;
protected:
	unsigned int textureId;
	TTexture() :textureId(0) {}
public:
	TTexture(std::string in_filename);
	TTexture(unsigned int textureId);
	TTexture(const TTexture& other);

	~TTexture();

	void Bind(int samplerIndex=0) const;
};


// Read PPM files (Week 9 lecture)
unsigned char *LoadPPM(const char *file, int *width, int *height, int *max);

std::string GetExt(std::string fileName);
