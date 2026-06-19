#include "cLightManager.h"

#include "../globalOpenGLStuff.h"	// Include just the OpenGL stuff

// 
// Call this once (per shader program)
void cLightManager::SetupUniformShaderLocations(unsigned int shaderProgramID)
{
	this->myLights[0].position_UL = glGetUniformLocation(shaderProgramID, "theLights[0].position");
	this->myLights[0].diffuse_UL = glGetUniformLocation(shaderProgramID, "theLights[0].diffuse");
	this->myLights[0].specular_UL = glGetUniformLocation(shaderProgramID, "theLights[0].specular");
	this->myLights[0].atten_UL = glGetUniformLocation(shaderProgramID, "theLights[0].atten");
	this->myLights[0].direction_UL = glGetUniformLocation(shaderProgramID, "theLights[0].direction");
	this->myLights[0].param1_UL = glGetUniformLocation(shaderProgramID, "theLights[0].param1");
	this->myLights[0].param2_UL = glGetUniformLocation(shaderProgramID, "theLights[0].param2");

	// Then for index #1...
	this->myLights[1].position_UL = glGetUniformLocation(shaderProgramID, "theLights[1].position");
	this->myLights[1].diffuse_UL = glGetUniformLocation(shaderProgramID, "theLights[1].diffuse");
	this->myLights[1].specular_UL = glGetUniformLocation(shaderProgramID, "theLights[1].specular");
	this->myLights[1].atten_UL = glGetUniformLocation(shaderProgramID, "theLights[1].atten");
	this->myLights[1].direction_UL = glGetUniformLocation(shaderProgramID, "theLights[1].direction");
	this->myLights[1].param1_UL = glGetUniformLocation(shaderProgramID, "theLights[1].param1");
	this->myLights[1].param2_UL = glGetUniformLocation(shaderProgramID, "theLights[1].param2");

	// And again for every other light uniform location you need to load...

	return;
}

// Called every frame
void cLightManager::CopyLightInfoToShader(unsigned int shaderProgramID)
{
	// Copy the C++ side structure data to the Light array in the shader

	// c++:		glm::vec3 position = glm::vec3(0.0f);		
	// GLSL:	vec4 position;	// xyz, ignoring w (4th parameter)	

	for (unsigned int lightIndex = 0; lightIndex < cLightManager::NUMBEROF_LIGHTS; lightIndex++)
	{
		//vec4 param2;	// x = 0 for off, 1 for on		glUniform4f
		glUniform4f( this->myLights[lightIndex].param2_UL, 
			         (this->myLights[lightIndex].bIsOn ? 1.0f : 0.0f ),
			         0.0f, 0.0f, 0.0f );	// Last 3 are not used at the moment

		// If light is off, don't bother passing the other things
		if ( this->myLights[lightIndex].bIsOn )
		{


			// vec4 position;	// xyz, ignoring w (4th parameter)	
			glUniform4f(this->myLights[lightIndex].position_UL,
						this->myLights[lightIndex].position.x,
						this->myLights[lightIndex].position.y,
		        		this->myLights[lightIndex].position.z,
						1.0f);		// If unsure what to set the 4th param, safest to set it to 1.0f

			// vec4 diffuse;
			glUniform4f( this->myLights[lightIndex].diffuse_UL,
						 this->myLights[lightIndex].diffuseRGBA.r,
						 this->myLights[lightIndex].diffuseRGBA.g,
						 this->myLights[lightIndex].diffuseRGBA.b,
						 this->myLights[lightIndex].diffuseRGBA.a );

			// vec4 specular;	// rgb = highlight colour, w = power
			glUniform4f( this->myLights[lightIndex].specular_UL, 
				         this->myLights[lightIndex].specularHighlightColour.r, 
				         this->myLights[lightIndex].specularHighlightColour.g,
				         this->myLights[lightIndex].specularHighlightColour.b, 
				         this->myLights[lightIndex].specularPower );

			// vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
			glUniform4f( this->myLights[lightIndex].atten_UL, 
				         this->myLights[lightIndex].attenuationConstant, 
				         this->myLights[lightIndex].attenuationLinear, 
				         this->myLights[lightIndex].attenuationQuadratic, 
				         this->myLights[lightIndex].attenuationDistanceCutoff );

			// vec4 direction;	// Spot, directional lights, ignoring w (4th parameter)
			glUniform4f( this->myLights[lightIndex].direction_UL, 
				         this->myLights[lightIndex].direction.x, 
				         this->myLights[lightIndex].direction.y, 
				         this->myLights[lightIndex].direction.z, 
				         1.0f );	// 4th not being used, so setting to 1.0f

			// vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
			glUniform4f( this->myLights[lightIndex].param1_UL, 
				         (GLfloat) this->myLights[lightIndex].lightType, 
				         this->myLights[lightIndex].spotLightInnerAngle, 
				         this->myLights[lightIndex].spotLightOuterAngle, 
				         1.0f );	// 4th not being used, so setting to 1.0f

		} // if ( this->myLights[lightIndex].bIsOn )

	}// for (unsigned int lightIndex = 0

	return;
}
