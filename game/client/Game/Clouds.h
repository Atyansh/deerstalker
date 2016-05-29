#pragma once

#include "client\Graphics\Shader.h"
#include "SGeode.h"
#include <vector>

using namespace std;

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define OFFSET_LOCATION 2
#define NONSES_LOCATION 3
#define SCALES_LOCATION 4
#define THRESHOLD_NORMALS_LOCATION 5
#define FRONT_POINTS_LOCATION 6
#define BACK_POINTS_LOCATION 7

#define M_PI 3.14159265359
#define M_PI_2 1.57079632679

class Clouds : public SGeode {
private:
	Shader *shader;
	//Cloud Shape
	float radius;
	unsigned int rings;
	unsigned int sectors;
	glm::vec3 center;
	
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<GLuint> indices;

	//cloud instance rendering
	//vector<glm::vec3> offsets;
	/*vector<GLfloat> nonses;
	vector<GLfloat> scales;
	vector<glm::vec3> thresholdNormals;
	vector<glm::vec3> frontPoints;
	vector<glm::vec3> backPoints;*/

	//Cloud time
	float prevTime = 0;
	float currCloudTime = 0;

	//location
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;
	

	/*  Render data  */
	GLuint VAO, EBO;
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	//GLuint offsetsBuffer;
	/*GLuint nonseBuffer, scaleBuffer, thresholdNormalsBuffer,
		frontPointsBuffer, backPointsBuffer;*/

	void setupCloud();
	void setUpBuffer();
	void setupUniformLoc();
	void bindBuffers();

public:
	Clouds(Shader *shader, float radius, int rings, int sectors, glm::vec3 center=glm::vec3(0.f,0.f,0.f));
	~Clouds();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
};