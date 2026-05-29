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
	glm::vec3 position;
	glm::vec3 rotation;
	float scale; 

	// std::string textures[8];
};
