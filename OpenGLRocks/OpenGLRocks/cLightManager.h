#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
//#include "globalStuff.h"	// For the OpenGL stuff

class cLight
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec4 diffuseRGBA = glm::vec4(1.0f);	

	glm::vec3 specularHighlightColour = glm::vec3(1.0f);
	float specularPower = 1.0f;

	// vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	float attenuationConstant = 0.0f;
	float attenuationLinear = 0.01f;
	float attenuationQuadratic = 0.001f;
	float attenuationDistanceCutoff = FLT_MAX;

	glm::vec3 direction = glm::vec3(0.0f);
	
	// vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	enum eLightType
	{
		POINT_LIGHT = 0,		// 0 = pointlight
		SPOT_LIGHT = 1,			// 1 = spot light
		DIRECTIONAL_LIGHT = 2	// 2 = directional light
	};
	eLightType lightType = POINT_LIGHT;
	float spotLightInnerAngle = 10.0f;
	float spotLightOuterAngle = 20.0f;
	
	//vec4 param2;	// x = 0 for off, 1 for on
	bool bIsOn = false;

	// The uniform location variables
	int position_UL = -1;			// GLint
	int diffuse_UL = -1;
	int specular_UL = -1;
	int atten_UL = -1;
	int direction_UL = -1;
	int param1_UL = -1;
	int param2_UL = -1;
};

class cLightManager
{
public:
	static const int NUMBEROF_LIGHTS = 8;
	cLight myLights[NUMBEROF_LIGHTS];

	// Call this once (per shader program)
	void SetupUniformShaderLocations(unsigned int shaderProgramID);
	// Called every frame
	void CopyLightInfoToShader(unsigned int shaderProgramID);

};

