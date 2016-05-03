#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;

out vec2 TexCoords;
out vec3 fragPosition;
out vec3 Normal;
out vec4 boneColor;

#define MAX_BONES 100

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones[MAX_BONES];
uniform int hasBones;

void main()
{
	vec4 newPos = vec4(position, 1.0f); ;
	boneColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    if (hasBones == 1 && weights != vec4(0.0f, 0.0f, 0.0f, 0.0f))
    {
        mat4 boneTransform = bones[boneIDs[0]] * weights[0];
        boneTransform += bones[boneIDs[1]] * weights[1];
        boneTransform += bones[boneIDs[2]] * weights[2];
        boneTransform += bones[boneIDs[3]] * weights[3];

        newPos = boneTransform * vec4(position, 1.0);
        vec4 test = vec4(1.0);
        boneColor = boneTransform * test;
        // newPos = boneTransform * newPos;
    }
	
    gl_Position = projection * view * model  * newPos;
    fragPosition = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}