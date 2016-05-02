#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;

out vec2 TexCoords;
out vec3 fragPosition;
out vec3 Normal;

#define MAX_BONES 100

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];
uniform int hasBones;

void main()
{
	mat4 BoneTransform = bones[boneIDs[0]] * weights[0];
    BoneTransform     += bones[boneIDs[1]] * weights[1];
    BoneTransform     += bones[boneIDs[2]] * weights[2];
    BoneTransform     += bones[boneIDs[3]] * weights[3];

	vec4 newPos;

	if (hasBones == 1) {
		newPos = /*BoneTransform * */vec4(position, 1.0f);
	} else {
		newPos = vec4(position, 1.0f); 
	}
	
    gl_Position = projection * view * model  * newPos;
    fragPosition = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}