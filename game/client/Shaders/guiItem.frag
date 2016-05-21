#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D guiItem;

void main() {
	color = texture(guiItem, TexCoords);
}