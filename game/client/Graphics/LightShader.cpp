#include "LightShader.h"

#include <glm/ext.hpp>

LightShader::LightShader() : Shader() {

}

LightShader::LightShader(const char *vs, const char *fs, bool isFile) : Shader(vs, fs, isFile) {

}


LightShader::~LightShader()
{

}

void LightShader::setDirectionalLight(){

}

void LightShader::addPointLight(){

}

void LightShader::addSpotLight(){

}

void LightShader::attachDirectionalLight(){
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.direction"), 1, glm::value_ptr(dirLight.direction));
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.ambient"), 1, glm::value_ptr(dirLight.ambient));
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.diffuse"), 1, glm::value_ptr(dirLight.diffuse));
	glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "dirLight.specular"), 1, glm::value_ptr(dirLight.specular));
}

void LightShader::attachPointLight(){
	for (int i = 0; i < pointLight.size(); i++){
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].position"), 1, glm::value_ptr(pointLight[i].position));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].ambient"), 1, glm::value_ptr(pointLight[i].ambient));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].diffuse"), 1, glm::value_ptr(pointLight[i].diffuse));
		glUniform3fv(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].specular"), 1, glm::value_ptr(pointLight[i].specular));
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].constant"), pointLight[i].constant);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].linear"), pointLight[i].linear);
		glUniform1f(glGetUniformLocation(this->currentlyBoundShaderID, "pointLights[0].quadratic"), pointLight[i].quadratic);
	}
}

void LightShader::attachSpotLight(){

}