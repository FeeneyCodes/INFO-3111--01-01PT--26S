#include "cVAOManager.h"

//#include "globalOpenGLStuff.h"
#include "../globalStuff.h"

#include <fstream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

#include <sstream>

sModelDrawInfo::sModelDrawInfo()
{
	this->VAO_ID = 0;

	this->VertexBufferID = 0;
	this->VertexBuffer_Start_Index = 0;
	this->numberOfVertices = 0;

	this->IndexBufferID = 0;
	this->IndexBuffer_Start_Index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// The "local" (i.e. "CPU side" temporary array)
	this->pVertices = 0;	// or NULL
	this->pIndices = 0;		// or NULL

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
//	glm::vec3 maxValues;
//	glm::vec3 minValues;

//	scale = 5.0/maxExtent;		-> 5x5x5
//	float maxExtent;

	return;
}

void cVAOManager::setBasePath(std::string newBasePath)
{
	this->m_FileBasePath = newBasePath;
	return;
}


bool cVAOManager::LoadModelIntoVAO(
		std::string fileName, 
		sModelDrawInfo &drawInfo,
	    unsigned int shaderProgramID)

{
	// Load the model from file
	// (We do this here, since if we can't load it, there's 
	//	no point in doing anything else, right?)

	drawInfo.meshName = fileName;
	
	std::string fullFileNameWithPath = this->m_FileBasePath + '/' + fileName;

	// Make sure that the LoadTheModel is loading the specific format 
	//	for *that* file. xyz, xyz+n, xyz+colour, xyz+colour+normal
	if ( ! this->m_LoadTheModel( fullFileNameWithPath, drawInfo ) )
	{
		this->m_AppendTextToLastError( "Didn't load model", true );
		return false;
	}

	// 
	// Model is loaded and the vertices and indices are in the drawInfo struct
	// 

	// Create a VAO (Vertex Array Object), which will 
	//	keep track of all the 'state' needed to draw 
	//	from this buffer...

	// Ask OpenGL for a new buffer ID...
	glGenVertexArrays( 1, &(drawInfo.VAO_ID) );
	// "Bind" this buffer:
	// - aka "make this the 'current' VAO buffer
	glBindVertexArray(drawInfo.VAO_ID);

	// Now ANY state that is related to vertex or index buffer
	//	and vertex attribute layout, is stored in the 'state' 
	//	of the VAO... 


	// NOTE: OpenGL error checks have been omitted for brevity
//	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &(drawInfo.VertexBufferID) );

//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
	// sVert vertices[3]
	glBufferData( GL_ARRAY_BUFFER, 
				  sizeof(sVert) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
				  (GLvoid*) drawInfo.pVertices,							// pVertices,			//vertices, 
				  GL_STATIC_DRAW );


	// Copy the index buffer into the video card, too
	// Create an index buffer.
	glGenBuffers( 1, &(drawInfo.IndexBufferID) );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
	              sizeof( unsigned int ) * drawInfo.numberOfIndices, 
	              (GLvoid*) drawInfo.pIndices,
                  GL_STATIC_DRAW );

	// Set the vertex attributes.


//	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");	// program
//	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");	// program;
//
//	// Set the vertex attributes for this shader
//	glEnableVertexAttribArray(vpos_location);	// vPos
//	glVertexAttribPointer( vpos_location, 
//		                   3,	// vPos
//						   GL_FLOAT, GL_FALSE,
//						   sizeof(sVert),				// Stride
//						   ( void* )offsetof(sVert, x));
//
//	glEnableVertexAttribArray(vcol_location);	// vCol
//	glVertexAttribPointer( vcol_location, 
//		                   3,		// vCol
//						   GL_FLOAT, GL_FALSE,
//						   sizeof(sVert), 
//						   ( void* )offsetof(sVert,r));

	// New vertex layout that's in the shader
	//	in vec4 vertexColour;		// RGBA
	//	in vec4 vertexPosition;		// XYZ (w not used)
	//	in vec4 vertexNormal;		// XYZ (w not used)
	//	in vec4 vertexUVx2;			// 2 sets of UVs (because it's a vec4)

	GLint vertexColour_location = glGetAttribLocation(shaderProgramID, "vertexColour");
	glEnableVertexAttribArray(vertexColour_location);	
	glVertexAttribPointer( vertexColour_location,
				           4,		// vCol
						   GL_FLOAT, GL_FALSE,
						   sizeof(sVert), 
						   ( void* )offsetof(sVert, vertColour.r));

	GLint vertexPosition_location = glGetAttribLocation(shaderProgramID, "vertexPosition");
	glEnableVertexAttribArray(vertexPosition_location);
	glVertexAttribPointer(vertexPosition_location,
	                       4,		// vCol
	                       GL_FLOAT, GL_FALSE,
	                       sizeof(sVert),
	                       (void*)offsetof(sVert, vertPosition.x));

	GLint vertexNormal_location = glGetAttribLocation(shaderProgramID, "vertexNormal");
	glEnableVertexAttribArray(vertexNormal_location);
	glVertexAttribPointer( vertexNormal_location,
	                       4,		// vCol
	                       GL_FLOAT, GL_FALSE,
	                       sizeof(sVert),
	                       (void*)offsetof(sVert, vertNormal.x));

	GLint vertexUVx2_location = glGetAttribLocation(shaderProgramID, "vertexUVx2");
	glEnableVertexAttribArray(vertexUVx2_location);
	glVertexAttribPointer( vertexUVx2_location,
	                       4,		// vCol
	                       GL_FLOAT, GL_FALSE,
	                       sizeof(sVert),
	                       (void*)offsetof(sVert, vertUVx2.x));

	// Now that all the parts are set up, set the VAO to zero
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vertexColour_location);
	glDisableVertexAttribArray(vertexPosition_location);
	glDisableVertexAttribArray(vertexNormal_location);
	glDisableVertexAttribArray(vertexUVx2_location);
//	glDisableVertexAttribArray(vpos_location);
//	glDisableVertexAttribArray(vcol_location);


	// Store the draw information into the map
	this->m_map_ModelName_to_VAOID[ drawInfo.meshName ] = drawInfo;


	return true;
}


// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(
		std::string filename,
		sModelDrawInfo &drawInfo) 
{
	std::map< std::string /*model name*/,
			sModelDrawInfo /* info needed to draw*/ >::iterator 
		itDrawInfo = this->m_map_ModelName_to_VAOID.find( filename );

	// Find it? 
	if ( itDrawInfo == this->m_map_ModelName_to_VAOID.end() )
	{
		// Nope
		return false;
	}

	// Else we found the thing to draw
	// ...so 'return' that information
	drawInfo = itDrawInfo->second;
	return true;
}


bool cVAOManager::m_LoadTheModel(std::string fileName,
								 sModelDrawInfo &drawInfo )
{
	// Open the file. 
	// Read until we hit the word "vertex"
	// Read until we hit the word "face"
	// Read until we hit the word "end_header"

	std::ifstream thePlyFile( fileName.c_str() );
	if ( ! thePlyFile.is_open() )
	{	// Something is wrong...
		std::stringstream ssError;
		ssError << "Can't open >" << fileName << "< file." << std::endl;
		this->m_AppendTextToLastError(ssError.str());
		return false;
	}

	std::string temp; 
	while ( thePlyFile >> temp )
	{
		if ( temp == "vertex" ) 
		{
			break;
		}
	}; 
	// read the number of vertices...
	thePlyFile >> drawInfo.numberOfVertices;	//::g_NumberOfVertices;

	while ( thePlyFile >> temp )
	{
		if ( temp == "face" ) 
		{
			break;
		}
	}; 
	// read the number of triangles...
	thePlyFile >> drawInfo.numberOfTriangles;		// ::g_NumberOfTriangles;


	while ( thePlyFile >> temp )
	{
		if ( temp == "end_header" ) 
		{
			break;
		}
	}; 

	// And now, we start reading vertices... Hazzah!
	
	// This is set up to match the ply (3d model) file. 
	// NOT the shader. 
	// This should match whatever the PLY file has
	//
	//	property float x	// position
	//	property float y
	//	property float z
	//	property float nx	// normal
	//	property float ny
	//	property float nz
	//	property uchar red		// diffuseRGBA
	//	property uchar green
	//	property uchar blue
	//	property uchar alpha
	//	property float texture_u	// TextureUV
	//	property float texture_v
	struct sVertPly
	{
		glm::vec3 position;
		glm::vec3 normal;		
		glm::vec4 diffuseRGBA;
		glm::vec2 textureUV;
	};

	std::vector<sVertPly> vecTempPlyVerts;

	sVertPly tempVert;
	// Load the vertices...
	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; // ::g_NumberOfVertices; 
		  index++ )
	{
		thePlyFile >> tempVert.position.x;
		thePlyFile >> tempVert.position.y; 
		thePlyFile >> tempVert.position.z;

		thePlyFile >> tempVert.normal.x;
		thePlyFile >> tempVert.normal.y;
		thePlyFile >> tempVert.normal.z;

		thePlyFile >> tempVert.diffuseRGBA.r;	
		thePlyFile >> tempVert.diffuseRGBA.b;
		thePlyFile >> tempVert.diffuseRGBA.g;
		thePlyFile >> tempVert.diffuseRGBA.a;

		// File has colours from 0-255 (HTML style), but 
		//	we want them from 0.0f to 1.0f
		tempVert.diffuseRGBA.r /= 255.0f;
		tempVert.diffuseRGBA.b /= 255.0f;
		tempVert.diffuseRGBA.g /= 255.0f;
		tempVert.diffuseRGBA.a /= 255.0f;

		thePlyFile >> tempVert.textureUV.x;
		thePlyFile >> tempVert.textureUV.y;


		//// HACK:
		//if (fileName == "assets/models/cow.ply")
		//{
		//	tempVert.positions.x /= 10.0f;
		//	tempVert.positions.y /= 10.0f;
		//	tempVert.positions.z /= 10.0f;
		//	// Skootch it 
		//	tempVert.positions.x -= 2.0f;


		//}
		//if (fileName == "assets/models/mig29_xyz_only.ply")
		//{
		//	tempVert.positions.x *= 2.0f;
		//	tempVert.positions.y *= 2.0f;
		//	tempVert.positions.z *= 2.0f;
		//	// Skootch it 
		//	tempVert.positions.x += 5.0f;
		//	tempVert.positions.y += 3.0f;
		//}



		//float garbageNumber;
		//thePlyFile >> garbageNumber;
		//thePlyFile >> garbageNumber;
		//thePlyFile >> garbageNumber;

		//thePlyFile >> tempVert.normals.x;
		//thePlyFile >> tempVert.normals.y;
		//thePlyFile >> tempVert.normals.z;		
		
		vecTempPlyVerts.push_back(tempVert);
	}

	// Create a local vertex array
	// Note: The format the file (ply) is DIFFERENT from this array:
	// - sVertPly was made to match the file format
	// - sVert was made to match the shader vertex attrib format


	drawInfo.pVertices = new sVert[drawInfo.numberOfVertices];

	// Optional clear array to zero 
	//memset( drawInfo.pVertices, 0, sizeof(sVert) * drawInfo.numberOfVertices);


	// The structure we had to load the PLY file is now converted 
	//	and copied into a structure that the shader expects...
	for ( unsigned int index = 0; index != drawInfo.numberOfVertices; index++ )
	{
		drawInfo.pVertices[index].vertPosition.x = vecTempPlyVerts[index].position.x;
		drawInfo.pVertices[index].vertPosition.y = vecTempPlyVerts[index].position.y;
		drawInfo.pVertices[index].vertPosition.z = vecTempPlyVerts[index].position.z;
		drawInfo.pVertices[index].vertPosition.w = 1.0f;

//		// The files doesn't have colours, so we'll set them to white
//		drawInfo.pVertices[index].r = 1.0f;	// getRand();			 //vecTempPlyVerts[index].colour.r;
//		drawInfo.pVertices[index].g = 1.0f;	// getRand();			//vecTempPlyVerts[index].colour.g;
//		drawInfo.pVertices[index].b = 1.0f;	// getRand();			//vecTempPlyVerts[index].colour.b;

		drawInfo.pVertices[index].vertColour.r = vecTempPlyVerts[index].diffuseRGBA.r;
		drawInfo.pVertices[index].vertColour.g = vecTempPlyVerts[index].diffuseRGBA.g;
		drawInfo.pVertices[index].vertColour.b = vecTempPlyVerts[index].diffuseRGBA.b;
		drawInfo.pVertices[index].vertColour.a = vecTempPlyVerts[index].diffuseRGBA.a;

		drawInfo.pVertices[index].vertNormal.x = vecTempPlyVerts[index].normal.x;
		drawInfo.pVertices[index].vertNormal.y = vecTempPlyVerts[index].normal.y;
		drawInfo.pVertices[index].vertNormal.z = vecTempPlyVerts[index].normal.z;
		drawInfo.pVertices[index].vertNormal.w = 1.0f;

		drawInfo.pVertices[index].vertUVx2.x = vecTempPlyVerts[index].textureUV.x;
		drawInfo.pVertices[index].vertUVx2.y = vecTempPlyVerts[index].textureUV.y;
		drawInfo.pVertices[index].vertUVx2.z = vecTempPlyVerts[index].textureUV.x;
		drawInfo.pVertices[index].vertUVx2.w = vecTempPlyVerts[index].textureUV.y;


	}// for ( unsigned int index...


	struct sTriPly
	{
		unsigned int vindex[3];		// the 3 indices
		sVertPly verts[3];			// The actual vertices
	};

	std::vector<sTriPly> vecTempPlyTriangles;
	sTriPly tempTriangle;
	unsigned int discard = 0;
	for ( unsigned int index = 0; index != drawInfo.numberOfTriangles;	// ::g_NumberOfTriangles; 
		  index++ )
	{
		// 3 5622 5601 5640
		thePlyFile >> discard						// the "3" at the start
			       >> tempTriangle.vindex[0]
			       >> tempTriangle.vindex[1]
			       >> tempTriangle.vindex[2];

		//// Look up the vertex that matches the triangle index values.
		//tempTriangle.verts[0] = vecTempPlyVerts[ tempTriangle.vindex[0] ];
		//tempTriangle.verts[1] = vecTempPlyVerts[ tempTriangle.vindex[1] ];
		//tempTriangle.verts[2] = vecTempPlyVerts[ tempTriangle.vindex[2] ];

		vecTempPlyTriangles.push_back( tempTriangle );
	}//for ( unsigned int index...

	
	// NOW, we need to put them into the vertex array buffer that 
	//	will be passed to OpenGL. Why? 
	// Because we called glDrawArrays(), that's why. 

//	::g_NumberOfVertsToDraw = ::g_NumberOfTriangles * 3;	// 3 because "triangles"

	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;

	// sVert* pVertices = 0;
//	pVertices = new sVert[::g_NumberOfVertsToDraw];

	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

	// Optional clear array to zero 
	//memset( drawInfo.pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices);

	// Allocate on the HEAP, so infinite size... 
	// delete pVertices			/// error!
	// delete [] pVertices		/// correct!!
	unsigned int indexBufferIndex = 0;
	for ( unsigned int triIndex = 0; 
		  triIndex != drawInfo.numberOfTriangles;		// ::g_NumberOfTriangles; 
		  triIndex++, indexBufferIndex += 3 )
	{
		sTriPly& curTri = vecTempPlyTriangles[triIndex];

		// This is taking an array of triangles vertex indices 
		// (which is a 2D array of indices, really) and converting
		//  it into a 1D array of indices...)
		drawInfo.pIndices[indexBufferIndex + 0] = curTri.vindex[0];
		drawInfo.pIndices[indexBufferIndex + 1] = curTri.vindex[1];
		drawInfo.pIndices[indexBufferIndex + 2] = curTri.vindex[2];

	}// for ( unsigned int triIndex = 0...

	return true;
}


std::string cVAOManager::getLastError(bool bAndClear /*=true*/)
{
	std::string theLastError = this->m_lastErrorString;

	if ( bAndClear )
	{
		this->m_lastErrorString = "";
	}

	return theLastError;
}

void cVAOManager::m_AppendTextToLastError(std::string text, bool addNewLineBefore /*=true*/)
{
	std::stringstream ssError;
	ssError << this->m_lastErrorString;

	if ( addNewLineBefore )
	{
		ssError << std::endl;
	}

	ssError << text;

	this->m_lastErrorString = ssError.str();

	return;

}
