#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include <vector>

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

	DirLight dirLight;
	vector<PointLight> pointLight;
	vector<SpotLight> spotLight;

public:

	LightShader();
	LightShader(const char *vs, const char *fs, bool isFile = true);
	~LightShader();

	void setDirectionalLight();
	void addPointLight();
	void addSpotLight();

	void attachDirectionalLight();
	void attachPointLight();
	void attachSpotLight();
};