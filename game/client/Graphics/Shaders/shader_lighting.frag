#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 boneColor;

out vec4 color;

uniform vec3 viewPos;
uniform int PointSize;
uniform int SpotSize;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform Material material;

uniform int hasTexture;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == ======================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == ======================================
    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Phase 2: Point lights
    for(int i = 0; i < PointSize && i < MAX_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // Phase 3: Spot light
	for(int i = 0; i < SpotSize && i < MAX_SPOT_LIGHTS; i++)
		result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);    
    
	color = vec4(result, 1.0);
	// color = boneColor;
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
    // Combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse;
	vec3 specular = light.specular;
	
	if (hasTexture == 1) {
		ambient *= vec3(texture(material.diffuse, TexCoords));
		diffuse *= diff * vec3(texture(material.diffuse, TexCoords));
		specular *= spec * vec3(texture(material.specular, TexCoords));
	}
	else {
		ambient *= vec3(1.0f, 1.0f, 1.0f);
		diffuse *= diff * vec3(1.0f, 1.0f, 1.0f);
		specular *= spec * vec3(1.0f, 1.0f, 1.0f);
	}
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// Combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse;
	vec3 specular = light.specular;
	
	if (hasTexture == 1) {
		ambient *= vec3(texture(material.diffuse, TexCoords));
		diffuse *= diff * vec3(texture(material.diffuse, TexCoords));
		specular *= spec * vec3(texture(material.specular, TexCoords));
	}
	else {
		ambient *= vec3(1.0f, 1.0f, 1.0f);
		diffuse *= diff * vec3(1.0f, 1.0f, 1.0f);
		specular *= spec * vec3(1.0f, 1.0f, 1.0f);
	}
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
	// Combine results
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse;
	vec3 specular = light.specular;
	
	if (hasTexture == 1) {
		ambient *= vec3(texture(material.diffuse, TexCoords));
		diffuse *= diff * vec3(texture(material.diffuse, TexCoords));
		specular *= spec * vec3(texture(material.specular, TexCoords));
	}
	else {
		ambient *= vec3(1.0f, 1.0f, 1.0f);
		diffuse *= diff * vec3(1.0f, 1.0f, 1.0f);
		specular *= spec * vec3(1.0f, 1.0f, 1.0f);
	}
	
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}