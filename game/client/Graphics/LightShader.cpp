#include "LightShader.h"

#include <glm/ext.hpp>

LightShader::LightShader() : Shader() {

}

LightShader::LightShader(glm::vec3 viewPos, const char *vs, const char *fs, bool isFile) : Shader(vs, fs, isFile) {
	this->viewPos = viewPos;
}


LightShader::~LightShader()
{

}

void LightShader::addDirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
	dirLight.direction = direction;
	dirLight.ambient = ambient;
	dirLight.diffuse = diffuse;
	dirLight.specular = specular;
}

void LightShader::addPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
	                            float constant, float linear, float quadratic){
	PointLight pointLight;
	pointLight.position = position;
	pointLight.ambient = ambient;
	pointLight.diffuse = diffuse;
	pointLight.specular = specular;
	pointLight.constant = constant;
	pointLight.linear = linear;
	pointLight.quadratic = quadratic;
	pointLights.push_back(pointLight);
}

void LightShader::addSpotLight(glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff,
							   glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
							   float constant, float linear, float quadratic){
	SpotLight spotLight;
	spotLight.position = position;
	spotLight.direction = direction;
	spotLight.cutOff = cutOff;
	spotLight.outerCutOff = outerCutOff;
	spotLight.ambient = ambient;
	spotLight.diffuse = diffuse;
	spotLight.specular = specular;
	spotLight.constant = constant;
	spotLight.linear = linear;
	spotLight.quadratic = quadratic;
	spotLights.push_back(spotLight);
}

void LightShader::bind()
{
	if (currentlyBoundShaderID != pid)
	{
		currentlyBoundShaderID = pid;
		glUseProgram(pid);
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "viewPos"), 1, glm::value_ptr(viewPos));
		attachDirectionalLight();
		attachPointLight();
		attachSpotLight();
	}
}

void LightShader::unbind()
{
	if (currentlyBoundShaderID != (0x0))
	{
		currentlyBoundShaderID = (0x0);
		glUseProgram(0);
	}
}

void LightShader::setViewPos(glm::vec3 viewPos){
	this->viewPos = viewPos;
}

void LightShader::attachDirectionalLight(){
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.direction"), 1, glm::value_ptr(dirLight.direction));
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.ambient"), 1, glm::value_ptr(dirLight.ambient));
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.diffuse"), 1, glm::value_ptr(dirLight.diffuse));
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.specular"), 1, glm::value_ptr(dirLight.specular));
}

void LightShader::attachPointLight(){
	glUniform1i(glGetUniformLocation(this->currentlyBoundShaderID, "PointSize"), pointLights.size());
	for (size_t i = 0; i < pointLights.size(); i++){
		string tmp = "pointLights[" + to_string(i) + "].";
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "position").c_str()), 1, glm::value_ptr(pointLights[i].position));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "ambient").c_str()), 1, glm::value_ptr(pointLights[i].ambient));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "diffuse").c_str()), 1, glm::value_ptr(pointLights[i].diffuse));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "specular").c_str()), 1, glm::value_ptr(pointLights[i].specular));
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "constant").c_str()), pointLights[i].constant);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "linear").c_str()), pointLights[i].linear);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "quadratic").c_str()), pointLights[i].quadratic);
	}
}

void LightShader::attachSpotLight(){
	glUniform1i(glGetUniformLocation(this->currentlyBoundShaderID, "SpotSize"), spotLights.size());
	for (size_t i = 0; i < spotLights.size(); i++){
		string tmp = "spotLights[" + to_string(i) + "].";
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "position").c_str()), 1, glm::value_ptr(spotLights[i].position));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "direction").c_str()), 1, glm::value_ptr(spotLights[i].direction));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "ambient").c_str()), 1, glm::value_ptr(spotLights[i].ambient));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "diffuse").c_str()), 1, glm::value_ptr(spotLights[i].diffuse));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "specular").c_str()), 1, glm::value_ptr(spotLights[i].specular));
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "constant").c_str()), spotLights[i].constant);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "linear").c_str()), spotLights[i].linear);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "quadratic").c_str()), spotLights[i].quadratic);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "cutOff").c_str()), spotLights[i].cutOff);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, (tmp + "outerCutOff").c_str()), spotLights[i].outerCutOff);
	}
}