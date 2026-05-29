// cMesh.h
#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

// GameObject
class cMesh
{
public:
	std::string meshName;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	float scale = 1.0f; 

	bool bIsWireFrame = false;

	// Colours
	glm::vec3 diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specularRGB_P;	

	// std::string textures[8];
};
