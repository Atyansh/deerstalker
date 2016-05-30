#version 330 core

out vec4 color;

in float noise;
in float transparency;
in vec3 normal;

//uniform vec3 lightDirection;
//uniform float lightIntensity;

float saturate(float x)
{
	float max = (x < 0)? 0 : x;
	float min = (max < 1)? max : 1;
	return min;
}

vec3 saturateV(vec3 v)
{
	return vec3(saturate(v.x), saturate(v.y), saturate(v.z));
}

void main()
{
	vec3 lightDirection = vec3(0,10,0); // change
	float lightIntensity = 0.5f; // change

	float saturatedTransparency = saturate(transparency);
	if (saturatedTransparency == 0) discard;

	vec3 sunColor = vec3(1,1,1); // change

	vec3 ambientIllumination = sunColor * (1.2 - noise * 0.6);
	vec3 diffuseIllumination = sunColor * saturate(dot(normal, lightDirection));
	//vec3 spectralIllumination = sunColor * pow(saturate(dot(reflection, -cloudDirection)), 5);
	diffuseIllumination = saturateV(diffuseIllumination);

	vec3 composedIllumination = max(sqrt(lightIntensity), 0.3) * 0.8 * ambientIllumination + sqrt(lightIntensity) * 0.25 * diffuseIllumination;

	color = vec4(composedIllumination , saturatedTransparency);
}