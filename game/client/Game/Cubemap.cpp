#include "Cubemap.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <boost/filesystem.hpp>
#include <SOIL/SOIL.h>

Cubemap::Cubemap() {

}


Cubemap::~Cubemap() {
}


/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* Cubemap::loadPPM(const char* filename, int& width, int& height)
{

	fprintf(stderr, "loading PPM\n");


	const int BUFSIZE = 128;
	FILE* fp;
	size_t read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	//Open the texture file
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

GLuint Cubemap::loadCubemap(const string& directory) {

	vector<const GLchar*> faces;
	/*faces.push_back("right.ppm");
	faces.push_back("left.ppm");
	faces.push_back("top.ppm");
	faces.push_back("bottom.ppm");
	faces.push_back("back.ppm");
	faces.push_back("front.ppm");*/

	faces.push_back("right.png");
	faces.push_back("left.png");
	faces.push_back("top.png");
	faces.push_back("bottom.png");
	faces.push_back("back.png");
	faces.push_back("front.png");

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	unsigned char* image;

	for (int i = 0; i < faces.size(); i++) {
		// image = loadPPM((directory + "/" + faces[i]).c_str(), width, height);
		image = SOIL_load_image((directory + "/" + faces[i]).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL) {
			cout << "error loading " << faces[i] << " from " << directory << endl;
			return 0;
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;

}
