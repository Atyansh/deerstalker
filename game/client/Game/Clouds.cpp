#include "Clouds.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>  
#include <algorithm>

Clouds::Clouds(Shader *shader, float radius, int rings, int sectors, glm::vec3 center) : SGeode()
{
	this->shader = shader;
	this->radius = radius;
	this->rings = rings;
	this->sectors = sectors;
	this->center = center;
	setupCloud();
	setUpBuffer();
	setupUniformLoc();
	createCloudPlane();
}
	

Clouds::~Clouds()
{
	
}

void Clouds::draw(DrawData& data) {

	float time = clock() / float(CLOCKS_PER_SEC);
	float delta = time - prevTime;
	prevTime = time;
	currCloudTime += min(delta, 0.05f);

	updateScene(min(delta, 0.05f), currCloudTime);
	shader->bind();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(data.matrix));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(data.view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(data.projection));
	glUniform3fv(expansionDirectionLoc, 1, glm::value_ptr(expansionDirection));
	glUniform1f(timeLoc, currCloudTime * MORPHING_SPEED_PER_SECOND);

	glBindVertexArray(this->VAO);
	bindBuffers();
	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL, offsets.size());
	glBindVertexArray(0);

	shader->unbind();
}

void Clouds::update(UpdateData& data) {
	
}

void Clouds::setupCloud() {
	/*Create Sphere*/
	float const R = 1.f / (float)(rings - 1);
	float const S = 1.f / (float)(sectors - 1);
	int r, s;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			vertices.push_back(glm::vec3(x, y, z)*radius + center);
			normals.push_back(glm::vec3(x, y, z));
			
		}
	}

	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		indices.push_back(r * sectors + s); // 1
		indices.push_back(r * sectors + (s + 1)); // 2
		indices.push_back((r + 1) * sectors + s); // 4

		indices.push_back(r * sectors + (s + 1)); // 2
		indices.push_back((r + 1) * sectors + (s + 1)); // 3
		indices.push_back((r + 1) * sectors + s); // 4
	}
}

void Clouds::setUpBuffer() {
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->verticesBuffer);
	glGenBuffers(1, &this->normalsBuffer);
	glGenBuffers(1, &this->offsetsBuffer);
	glGenBuffers(1, &this->noiseBuffer);
	glGenBuffers(1, &this->scaleBuffer);
	
}

void Clouds::setupUniformLoc(){
	if (!shader->isInitilized()) {
		cerr << "Shader not initialized" << endl;
		exit(-1);
	}

	shader->bind();

	modelLoc = glGetUniformLocation(shader->getPid(), "model");
	viewLoc = glGetUniformLocation(shader->getPid(), "view");
	projLoc = glGetUniformLocation(shader->getPid(), "projection");
	expansionDirectionLoc = glGetUniformLocation(shader->getPid(), "expansionDirection");
	timeLoc = glGetUniformLocation(shader->getPid(), "time");

	shader->unbind();
}

void Clouds::bindBuffers() {
	 // Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->offsetsBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->offsets.size() * sizeof(glm::vec3), &this->offsets[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(OFFSET_LOCATION);
	glVertexAttribPointer(OFFSET_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribDivisor(OFFSET_LOCATION, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->noiseBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->noises.size(), &this->noises[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NOISES_LOCATION);
	glVertexAttribPointer(NOISES_LOCATION, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(NOISES_LOCATION, 1);

	glBindBuffer(GL_ARRAY_BUFFER, this->scaleBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->scales.size(), &this->scales[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(SCALES_LOCATION);
	glVertexAttribPointer(SCALES_LOCATION, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribDivisor(SCALES_LOCATION, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
	
}

//cloud creation
void Clouds::updateScene(float passedSeconds, float totalPassedTime)
{
	// move the spheres
	for (size_t i = 0; i < offsets.size(); i++)
	{
		// translate the offset of all spheres
		// - floats at same speed no matter which FPS
		offsets.at(i) += expansionDirection * FLOATING_SPEED_PER_SECOND * passedSeconds;

		// remove if it is out of view (2D cloud offset > radius of cloud plane circle)
		float offsetLength = glm::length(glm::vec2(offsets.at(i).x, offsets.at(i).z));
		bool invisible = offsetLength > SKYBOX_LENGTH * 1.4; // add 40% to avoid removing clouds that have just been spawned
		if (invisible)
		{
			offsets.erase(offsets.begin() + i);
			noises.erase(noises.begin() + i);
			scales.erase(scales.begin() + i);
			i--;
		}
	}

	// add new spheres
	if (offsets.size() < NUMBER_OF_SPHERES - SPHERES_PER_CLOUD/* && currentFrameRate > MIN_FRAME_RATE*/)
	{

		int numberOfAdditionalClouds = (NUMBER_OF_SPHERES - offsets.size()) / SPHERES_PER_CLOUD;
		for (size_t i = 0; i < numberOfAdditionalClouds; i++)
		{
			float randDecision = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if ((totalPassedTime - lastCloudCreated > MIN_CLOUD_CREATION_DISTANCE_SECONDS && randDecision < SPAWN_CLOUD_LIKELYHOOD)
				|| offsets.size() == 0)
			{
				addCloud(SPHERES_PER_CLOUD);
				lastCloudCreated = totalPassedTime;
			}
		}
	}
}

void Clouds::addSphere(glm::vec3 offset)
{
	// add a sphere on the outline of the circular cloud plane
	float xRand2D = -0.5f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // [-0.5, 0.5]
	float yRand2D = -0.5f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // [-0.5, 0.5]
																						   // create the plane for the clouds
	glm::vec2 planarExpansionDirection(expansionDirection.x, expansionDirection.z);
	planarExpansionDirection = normalize(planarExpansionDirection);
	// randomize the start position
	planarExpansionDirection.x += xRand2D;
	planarExpansionDirection.y += yRand2D;
	planarExpansionDirection = normalize(planarExpansionDirection);
	// scale the plane up to the size of the sky (+10% to make clouds float into the scene)
	planarExpansionDirection = -planarExpansionDirection * SKYBOX_LENGTH * 1.3f;

	float rX = planarExpansionDirection.x;
	float rY = CLOUD_PLANE_HEIGHT;
	float rZ = planarExpansionDirection.y;

	offsets.insert(offsets.begin(), glm::vec3(rX, rY, rZ));

	noises.insert(noises.begin(), NOISE_FACTOR * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // [0,NONSE_FACTOR]
	scales.insert(scales.begin(), MIN_CLOUD_SCALE + (MAX_CLOUD_SCALE - MIN_CLOUD_SCALE) * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // [min,max]
}

void Clouds::addCloud(int numberOfSpheres)
{
	glm::vec3 offset(0);
	for (size_t i = 0; i < numberOfSpheres; i++)
	{
		addSphere(offset);
	}
}

void Clouds::createCloudPlane() {
	for (int i = 0; i < SKYBOX_LENGTH / 2+100; i++) {
		// add a sphere on the outline of the circular cloud plane
		float xRand2D = -0.5f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // [-0.5, 0.5]
		float yRand2D = -0.5f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // [-0.5, 0.5]

		float rX = xRand2D * SKYBOX_LENGTH * 2.4;
		float rY = CLOUD_PLANE_HEIGHT;
		float rZ = yRand2D * SKYBOX_LENGTH * 2.0;

		offsets.insert(offsets.begin(), glm::vec3(rX, rY, rZ));

		noises.insert(noises.begin(), NOISE_FACTOR * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // [0,NONSE_FACTOR]
		scales.insert(scales.begin(), MIN_CLOUD_SCALE + (MAX_CLOUD_SCALE - MIN_CLOUD_SCALE) * static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // [min,max]
	}
}
