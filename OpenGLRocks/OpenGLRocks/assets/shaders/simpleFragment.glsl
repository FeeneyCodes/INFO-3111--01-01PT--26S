#version 330

in vec3 color;

out vec4 fragment;	// to the screen

void main()
{
	// This is the colour of the pixel
	vec3 theColour = color;
	// Trick the compiler
	theColour *= 0.00001f;	// Effectively gone
	
	theColour.r += 1.0f;
	theColour.g += 0.0f;
	theColour.b += 0.0f;
	
    fragment = vec4(theColour, 1.0);
}