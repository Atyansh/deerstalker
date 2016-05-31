#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	vec4 tColor = vec4(textColor, 1.0) * sampled;
	// Plan is that text will have alpha of 1.0f. If anything less, then don't show
	if (tColor.a != 1.0f) {
		discard;
	}
    color = tColor;
}  