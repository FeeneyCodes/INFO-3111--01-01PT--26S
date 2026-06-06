#version 330

uniform mat4 MVP;	

//in vec3 vCol;
//in vec3 vPos;
in vec4 vertexColour;		// RGBA
in vec4 vertexPosition;		// XYZ (w not used)
in vec4 vertexNormal;		// XYZ (w not used)
in vec4 vertexUVx2;			// 2 sets of UVs (because it's a vec4)

//out vec3 color;		// to the fragment shader
out vec4 vModelColour;
out vec4 vNormal;
out vec4 vWorldPosition;
out vec4 vVertexUVx2;

void main()
{
	// This is the position of the vertes ON SCREEN
    gl_Position = MVP * vec4(vertexPosition.xyz, 1.0);
    
	// We are going to do something with these next week...
	// ...but for now, we'll just pass them along
	vNormal = vertexNormal;
	vWorldPosition = gl_Position;
	
	// Pass unchanged to the fragment shader
	vModelColour = vertexColour;
	vVertexUVx2 = vertexUVx2;
}
