#version 330 core
in vec3 thecolor;
out vec4 color;

uniform vec3 objectColor;

void main()
{    
    color = vec4(thecolor, 1.0);
}  