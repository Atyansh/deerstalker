#version 330 core
layout (location = 0) in vec2 vertex; // <vec2 position>
layout (location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 projection;

out vec3 thecolor;

void main()
{
    gl_Position = projection * model * vec4(vertex, 0.0, 1.0);
	thecolor = color;
}