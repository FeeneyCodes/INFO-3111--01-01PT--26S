#version 330

in vec3 color;

out vec4 fragment;	// to the screen

uniform vec3 theColour;

void main()
{
	// This is the colour of the pixel
	vec3 pixelColour = color;
	// Trick the compiler
	pixelColour.rgb *= 0.00001f;	// Effectively gone
	
	pixelColour.rgb += theColour.rgb;
	
    fragment = vec4(pixelColour, 1.0);
}