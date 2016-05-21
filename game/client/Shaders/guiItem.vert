#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	//gl_Position = vec4(1.0f);
	gl_Position = projection * view * vec4(position, 1.0);
	// We swap the y-axis by substracting our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	TexCoords = vec2(texCoord.x, 1.0 - texCoord.y);
}