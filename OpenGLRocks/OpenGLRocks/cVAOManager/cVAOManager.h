#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

// The vertex structure, as it is in the SHADER (on the GPU)
// This is also called the 'vertex layout'. 
// This means that if the shader vertex layout changed, this
//  vertex structure will also need to change (usually). 
//
// in vec3 vCol;
// in vec3 vPos;
//struct sVert
//{
//	float x, y, z;		// vPos
//	float r, g, b;		// vCol
//};

// Now our vertex shader has a vertex layout like this:
//	in vec4 vertexColour;		// RGBA
//	in vec4 vertexPosition;		// XYZ (w not used)
//	in vec4 vertexNormal;		// XYZ (w not used)
//	in vec4 vertexUVx2;			// 2 sets of UVs (because it's a vec4)
struct sVert
{
	glm::vec4 vertColour;		//	in vec4 vertexColour;		// RGBA
	glm::vec4 vertPosition;		//	in vec4 vertexPosition;		// XYZ (w not used)
	glm::vec4 vertNormal;		//	in vec4 vertexNormal;		// XYZ (w not used)
	glm::vec4 vertUVx2;			//	in vec4 vertexUVx2;			// 2 sets of UVs (because it's a vec4)
};

// This represents a single mesh (3D object) structure in 'indexed' format. 
// Its layout is set up to match how the GPU sees the mesh, rather
//  than how the file was, etc. 
// It's also storing the infomation that we need to tell the GPU
//  which model we want to draw. 
struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	// THE ONE THAT's IN THE FORMAT THE ++SHADER++ wants
	sVert* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;
};


class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// you Could do this where it doesn't return the draw info object
	bool LoadModelIntoVAO(std::string fileName, 
						  unsigned int shaderProgramID);

	//bool LoadModelIntoVAO_XYZ_Normals(std::string fileName, 
	//					  sModelDrawInfo &drawInfo, 
	//					  unsigned int shaderProgramID);

	//bool LoadModelIntoVAO_XYZ_Normals_Colours(std::string fileName, 
	//					  sModelDrawInfo &drawInfo, 
	//					  unsigned int shaderProgramID);


	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);
	
	void setBasePath(std::string newBasePath);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// Loads the ply model file into a temporary array
	bool m_LoadTheModel( std::string fileName, 
						 sModelDrawInfo &drawInfo);

	std::string m_lastErrorString;
	void m_AppendTextToLastError(std::string text, bool addNewLineBefore = true);
	
	std::string m_FileBasePath = "";
};

#endif	// _cVAOManager_HG_
