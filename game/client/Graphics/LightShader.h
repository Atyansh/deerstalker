#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>
#include <iostream>

using namespace std;

class LightShader : public Shader {
private:
	struct DirLight {
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct PointLight {
		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;

		float constant;
		float linear;
		float quadratic;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	glm::vec3 viewPos;
	DirLight dirLight;
	vector<PointLight> pointLights;
	vector<SpotLight> spotLights;

	void attachDirectionalLight();
	void attachPointLight();
	void attachSpotLight();

public:

	LightShader();
	LightShader(glm::vec3 viewPos, const char *vs, const char *fs, bool isFile = true);
	~LightShader();

	void setViewPos(glm::vec3 viewPos);
	void addDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void addPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
					   float constant, float linear, float quadratic);
	void addSpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, 
					  glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
					  float constant, float linear, float quadratic);

	virtual void bind();
	virtual void unbind();
};