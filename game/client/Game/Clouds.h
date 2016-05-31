#pragma once

#include "client\Graphics\Shader.h"
#include "SGeode.h"
#include <vector>

using namespace std;

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define OFFSET_LOCATION 2
#define NOISES_LOCATION 3
#define SCALES_LOCATION 4

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
	vector<glm::vec3> offsets;
	vector<GLfloat> noises;
	vector<GLfloat> scales;

	//Cloud time
	float prevTime = 0;
	float currCloudTime = 0;
	float lastCloudCreated = 0;

	//location
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;
	GLuint expansionDirectionLoc;
	GLuint timeLoc;
	

	/*  Render data  */
	GLuint VAO, EBO;
	GLuint verticesBuffer;
	GLuint normalsBuffer;
	GLuint offsetsBuffer;
	GLuint noiseBuffer, scaleBuffer;

	void setupCloud();
	void setUpBuffer();
	void setupUniformLoc();
	void bindBuffers();

	//cloud sky creation
	const int NUMBER_OF_SPHERES = 2000;
	const int SPHERES_PER_CLOUD = 1;
	const float FLOATING_SPEED_PER_SECOND = 30.0f;
	const float SKYBOX_LENGTH = 1000; //pass in?
	const glm::vec3 INITIAL_EXPANSION_DIRECTION = glm::vec3(1.0f, 0.0f, 1.0f);
	const float MIN_CLOUD_CREATION_DISTANCE_SECONDS = .05f;
	const float SPAWN_CLOUD_LIKELYHOOD = 0.35f * SPHERES_PER_CLOUD / NUMBER_OF_SPHERES;
	const float CLOUD_PLANE_HEIGHT = SKYBOX_LENGTH * 0.1f;
	const float NOISE_FACTOR = 100;
	const float MAX_CLOUD_SCALE = 10.0f;
	const float MIN_CLOUD_SCALE = 4.0f;
	const float MORPHING_SPEED_PER_SECOND = .1f;

	glm::vec3 expansionDirection = glm::vec3(1.0f, 0.0f, 0.0f);

	void updateScene(float passedSeconds, float totalPassedTime);
	void addCloud(int numberOfSpheres);
	void addSphere(glm::vec3 offset);
	void createCloudPlane();
public:
	Clouds(Shader *shader, float radius, int rings, int sectors, glm::vec3 center=glm::vec3(0.f,0.f,0.f));
	~Clouds();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
};