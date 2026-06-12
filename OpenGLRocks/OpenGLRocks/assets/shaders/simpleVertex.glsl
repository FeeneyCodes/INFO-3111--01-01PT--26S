#version 330

//uniform mat4 MVP;	
uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProject;

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
	mat4 matMVP = mProject * mView * mModel;
	
    gl_Position = matMVP * vec4(vertexPosition.xyz, 1.0f);
    
	// Multiply by the "inverse transpose" of the world matrix to 
	//	leave only rotational transformations
	vNormal = inverse(transpose(mModel)) * vertexNormal;

	// World position of vertex is just using the world (model) matrix
	vWorldPosition = mModel * vec4(vertexPosition.xyz, 1.0f);
	
	// Pass unchanged to the fragment shader
	vModelColour = vertexColour;
	vVertexUVx2 = vertexUVx2;
}
