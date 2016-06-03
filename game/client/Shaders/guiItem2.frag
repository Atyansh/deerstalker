#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;

void main()
{    
    color = texture(image, TexCoords);
}  

/*#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D guiItem;

void main() {
	vec4 tColor = texture(guiItem, TexCoords);
	
	//vec4 tColor = vec4(textColor, 1.0) * guiItem;
	// Plan is that text will have alpha of 1.0f. If anything less, then don't show
	//if (tColor.a < 0.1f) {
	//	discard;
	//}
    color = tColor;
}*/