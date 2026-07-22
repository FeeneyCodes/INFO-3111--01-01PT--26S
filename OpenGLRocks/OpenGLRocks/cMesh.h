// cMesh.h
#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

// GameObject
class cMesh
{
public:
	std::string meshName;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	float scale = 1.0f; 

	bool bIsWireFrame = false;

	std::string friendlyName;	
	bool bIsVisible = true;
	bool bDoNotLight = false;

	float alphaTransparency = 1.0f;	// 0.0f = invisible, 1.0f = opaque

	// Colours
	glm::vec3 diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specularRGB_P;

	bool bUseTexture = false;
	std::string textureName = "";

	// std::string textures[8];
	//std::vector< cMesh* > m_vec_pChildMeshes;
};
