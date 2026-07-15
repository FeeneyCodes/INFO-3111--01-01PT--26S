//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
////#include "include/glad/glad.h"
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include <iostream>
#include <fstream>      // File
#include <sstream>      // String stream

//#include "linmath.h"      // Another math library we aren't using
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>

#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cLightHelper.h"

#include "cMesh.h"

struct Vertex
{
    glm::vec3 position;      // vec2 pos;  position
    glm::vec3 colour;        // vec3 col;    colour
};

//Vertex vertices[6] =
//{   //     X     Y       Z    R      G     B  
//    //   float   float         float 
//    { { -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 1.0f } },
//    { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
//    { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
//    { { -0.6f + 2.0f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
//    { {  0.6f + 2.0f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
//    { {  0.0f + 2.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
//};

// Where our vertices are
//Vertex* pVerticesBunny = NULL;   // = new [numberpfVertices]
//unsigned long numberOfVertices = 0;


//glm::vec3 g_eyePosition = glm::vec3(0.0f, 0.0f, -5.0f);   // Camera location or position
//glm::vec3 g_atPosition = glm::vec3(0.0f, 0.0f, 0.0f);// Looking "at" 
glm::vec3 g_upAxis = glm::vec3(0.0f, +1.0f, 0.0f);// What's up

//
cVAOManager* g_pVAOManager = NULL;

cBasicFlyCamera* g_pFlyCamera = NULL;

cLightManager* g_pLightManager = NULL;

bool g_pShowDebugLightSpheres = true;

// note these are pointers
std::vector< cMesh* > g_vec_pMeshes;

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Set the callbacks for the mouse
// https://www.glfw.org/docs/3.3/input_guide.html#input_mouse
//
// Set with glfwSetCursorPosCallback(window, cursor_position_callback);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//
// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered);
//
// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//
// Set with glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// These are in mouse_keyboard_async.cpp
void handleKeyboardAsync(GLFWwindow* window);
void handleMouseAsync(GLFWwindow* window);

// In DrawObject.cpp
void DrawObject(cMesh* pTheMesh, GLuint program);

// Load a ply file and put it into the pVertice array
//void LoadAModelFromFile(std::string fileName);

int main(void)
{
    // Compile time "c style" array 
//    Vertex vertices2[1000];       // STACK   28000
    // Dynamic array in C++
//    Vertex* pVertices = new Vertex[10000000];   // HEAP
    //int x = 0;
    //int* pX = new int();
    //int* pY = new int[1000];

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core sometimes doesn't work on some cards (gives black screen)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    


    GLFWwindow* window = glfwCreateWindow(1024, 768, "I love OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // This is the "typing" style callback
    // PRESS, RELEASE, REPEAT
    glfwSetKeyCallback(window, key_callback);
    // And the mouse callbacks
    glfwSetCursorPosCallback(window, cursor_position_callback);   
    glfwSetCursorEnterCallback(window, cursor_enter_callback);    
    glfwSetMouseButtonCallback(window, mouse_button_callback);    
    glfwSetScrollCallback(window, scroll_callback);



    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    std::cout << glfwGetVersionString() << std::endl;
    GLint GLMajor, GLMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &GLMajor);
    glGetIntegerv(GL_MINOR_VERSION, &GLMinor);
    std::cout << "OpenGL Version " << GLMajor << "." << GLMinor << std::endl;

    // Load the model
//    LoadAModelFromFile("assets/models/mig29.ply");
//    LoadAModelFromFile("assets/models/de--lorean.ply");



    //GLuint vertex_buffer;
    //glGenBuffers(1, &vertex_buffer);
    //glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    //unsigned long numberOfBytesInArray
    //            = sizeof(Vertex) * numberOfVertices;

    //// Copy the array to the GPUs RAM
    //glBufferData( GL_ARRAY_BUFFER, 
    //              numberOfBytesInArray,     // sizeof(vertices),
    //              pVertices,                // vertices,
    //              GL_STATIC_DRAW);


    cShaderManager* pTheShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "simpleVertex.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "simpleFragment.glsl";

    pTheShaderManager->setBasePath("assets/shaders/");

    if (!pTheShaderManager->createProgramFromFile("SimpleShader",
        vertexShader,
        fragmentShader))
    {
        std::cout << "Shader error:" << pTheShaderManager->getLastError() << std::endl;
        return -1;
    }

    // Get the shader program ID (for later)
    GLuint program = pTheShaderManager->getIDFromFriendlyName("SimpleShader");


    glUseProgram(program);


    // Load the models
    ::g_pVAOManager = new cVAOManager();

    ::g_pVAOManager->setBasePath("assets/models");

    sModelDrawInfo carModel;
    if (::g_pVAOManager->LoadModelIntoVAO("de--lorean_xyz_n_rgba_uv.ply", carModel, program))
    {
        std::cout << "Loaded " << carModel.meshName << std::endl;
        std::cout << "\t" << carModel.numberOfVertices << " vertices" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load " << carModel.meshName << std::endl;
    }

    sModelDrawInfo cowModel;
    ::g_pVAOManager->LoadModelIntoVAO("cow_xyz_n_rgba_uv.ply", cowModel, program);

    sModelDrawInfo mig29Model;
    ::g_pVAOManager->LoadModelIntoVAO("mig29_xyz_n_rgba_uv.ply", mig29Model, program);

    sModelDrawInfo bunnyModel;
    ::g_pVAOManager->LoadModelIntoVAO("bun_zipper_xyz_n_rgba_uv.ply", bunnyModel, program);

     sModelDrawInfo terrainModel;
    ::g_pVAOManager->LoadModelIntoVAO("terrain_xyz_n_rgba_uv.ply", terrainModel, program);

     sModelDrawInfo warehouseModel;
    ::g_pVAOManager->LoadModelIntoVAO("Warehouse_xyz_n_rgba_UV (Blender Smart UV project).ply", warehouseModel, program);



    ::g_pVAOManager->setBasePath("assets/models/Dungeon_models/Additions");
     sModelDrawInfo treeModel;
//    ::g_pVAOManager->LoadModelIntoVAO("SM_Env_Camp_Well_01.ply", wellModel, program);
    ::g_pVAOManager->LoadModelIntoVAO("SM_Env_Tree_Big_01.obj.ply", treeModel, program);


    ::g_pVAOManager->setBasePath("assets/models");
    // Also load the light debug spheres...
     sModelDrawInfo debugSphere01Model;
    ::g_pVAOManager->LoadModelIntoVAO("Isoshphere_flat_3div_xyz_n_rgba_uv.ply", 
                                      debugSphere01Model, program);

     sModelDrawInfo debugSphere02Model;
    ::g_pVAOManager->LoadModelIntoVAO("Isoshphere_flat_4div_xyz_n_rgba_uv.ply", 
                                      debugSphere02Model, program);

     sModelDrawInfo debugSphere03Model;
    ::g_pVAOManager->LoadModelIntoVAO("Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply", 
                                      debugSphere03Model, program);

    cMesh* pDebugSphere01 = new cMesh();
    pDebugSphere01->meshName = "Isoshphere_flat_3div_xyz_n_rgba_uv.ply";
    pDebugSphere01->friendlyName = "DebugSphere1";
    pDebugSphere01->scale = 3.0f;
    pDebugSphere01->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    pDebugSphere01->bIsVisible = false;

    cMesh* pDebugSphere02 = new cMesh();
    pDebugSphere02->meshName = "Isoshphere_flat_4div_xyz_n_rgba_uv.ply";
    pDebugSphere02->friendlyName = "DebugSphere2";
    pDebugSphere02->bIsVisible = false;

    cMesh* pDebugSphere03 = new cMesh();
    pDebugSphere03->meshName = "Isoshphere_smooth_inverted_normals_xyz_n_rgba_uv.ply";
    pDebugSphere03->friendlyName = "DebugSphere3";
    pDebugSphere03->bIsVisible = false;

    g_vec_pMeshes.push_back(pDebugSphere01);
    g_vec_pMeshes.push_back(pDebugSphere02);
    g_vec_pMeshes.push_back(pDebugSphere03);



    //cMesh* pWell = new cMesh();
    //pWell->meshName = "SM_Env_Camp_Well_01.ply";
    //pWell->bIsWireFrame = true;
    //pWell->scale = 0.1f;
    //pWell->position.z = 50.0f;
    //::g_vec_pMeshes.push_back(pWell);

    cMesh* pTree = new cMesh();
    pTree->meshName = "SM_Env_Tree_Big_01.obj.ply";
    pTree->diffuseRGB = glm::vec3(0.0f, 1.0f, 0.0f);
    pTree->scale = 0.1f;
    pTree->position.z = 30.0f;
    ::g_vec_pMeshes.push_back(pTree);


    cMesh* pTerrain = new cMesh();
    pTerrain->meshName = "terrain_xyz_n_rgba_uv.ply";
    pTerrain->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
    //pTerrain->bIsWireFrame = true;
    pTerrain->rotation.x = -90.0f;
    pTerrain->position.y = -50.0f;
    ::g_vec_pMeshes.push_back(pTerrain);

    // Add the models we want to draw
    cMesh* pCar = new cMesh();
    pCar->meshName = "de--lorean_xyz_n_rgba_uv.ply";
    pCar->position = glm::vec3(0.0f, -8.0f, 0.0f);
    pCar->rotation.x = -90.0f;
    pCar->rotation.y = 90.0f;
    pCar->scale = 1.0f / 2.0f;
    //pCar->bIsWireFrame = true;
    pCar->diffuseRGB = glm::vec3(0.0f, 0.0f, 1.0f);

    // Add the models we want to draw



    cMesh* pPlane = new cMesh();
    pPlane->meshName = "mig29_xyz_n_rgba_uv.ply";
    pPlane->position = glm::vec3(0.0f, 2.0f, 0.0f);
    pPlane->rotation.x = 45.0f;
    pPlane->rotation.y = 15.0f;
    pPlane->scale = 2.0f;
    pPlane->diffuseRGB = glm::vec3(1.0f, 0.0f, 0.0f);

    cMesh* pBunny1 = new cMesh();
    pBunny1->meshName = "bun_zipper_xyz_n_rgba_uv.ply";
    pBunny1->position.x = 2.0f;
    pBunny1->diffuseRGB = glm::vec3(0.6f, 0.3f, 0.7f);

    cMesh* pBunny2 = new cMesh();
    pBunny2->meshName = "bun_zipper_xyz_n_rgba_uv.ply";
    pBunny2->scale = 5.0f;
    pBunny2->position.x = +10.0f;
    pBunny1->diffuseRGB = glm::vec3(0.3f, 0.7f, 0.6f);


    cMesh* pWarehouse = new cMesh();
    pWarehouse->meshName = "Warehouse_xyz_n_rgba_UV (Blender Smart UV project).ply";
    pWarehouse->diffuseRGB = glm::vec3(0.7f, 0.7f, 0.7f);
    pWarehouse->position = glm::vec3(0.0f, -15.0f, 0.0f);


    
    ::g_vec_pMeshes.push_back(pPlane);
    ::g_vec_pMeshes.push_back(pBunny1);
    ::g_vec_pMeshes.push_back(pBunny2);
    ::g_vec_pMeshes.push_back(pCar);
    ::g_vec_pMeshes.push_back(pWarehouse);

    for (float z = -100.0f; z < 101.0f; z += 10.0f)
    {
        cMesh* pCow = new cMesh();
        pCow->meshName = "cow_xyz_n_rgba_uv.ply";
        pCow->position = glm::vec3(0.0f, 0.0f, z);
        pCow->scale = 1.0f;
        pCow->bIsWireFrame = false;
        pCow->diffuseRGB = glm::vec3(getRand(), getRand(), getRand());
        pCow->alphaTransparency = 0.6f;

        ::g_vec_pMeshes.push_back(pCow);
    }



    // Create a camera
    ::g_pFlyCamera = new cBasicFlyCamera();
    ::g_pFlyCamera->setEyeLocation(0.0f, 0.0f, -5.0f);


    // Light manager
    ::g_pLightManager = new cLightManager();
    ::g_pLightManager->SetupUniformShaderLocations(program);

    //// Set up the lights
    //::g_pLightManager->myLights[0].bIsOn = true;
    //::g_pLightManager->myLights[0].lightType = cLight::POINT_LIGHT;
    //// Place light 20 units above the origin
    //::g_pLightManager->myLights[0].position = glm::vec3(0.0f, 20.0f, 0.0f);
    //// Bright white light
    //::g_pLightManager->myLights[0].diffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    //::g_pLightManager->myLights[0].attenuationConstant = 0.0f;
    //::g_pLightManager->myLights[0].attenuationLinear = 0.01f;
    //::g_pLightManager->myLights[0].attenuationQuadratic = 0.001f;


    //::g_pLightManager->myLights[1].bIsOn = true;
    //::g_pLightManager->myLights[1].lightType = cLight::POINT_LIGHT;
    //::g_pLightManager->myLights[1].position = glm::vec3(-10.0f, -10.0f, 0.0f);
    //// Bright white light
    //::g_pLightManager->myLights[1].diffuseRGBA = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

    //::g_pLightManager->myLights[1].attenuationConstant = 0.0f;
    //::g_pLightManager->myLights[1].attenuationLinear = 0.02f;
    //::g_pLightManager->myLights[1].attenuationQuadratic = 0.01f;

    // Setting up spot light
    cLight* pSpotLight = &(::g_pLightManager->myLights[0]);
    pSpotLight->lightType = cLight::SPOT_LIGHT;
    pSpotLight->bIsOn = true;
    pSpotLight->position = glm::vec3(0.0f, 20.0f, 0.0f);
    pSpotLight->direction = glm::vec3(0.0f, -1.0f, 0.0f);
    // NEVER FORGET TO NORMALIZE DIRECTION
	pSpotLight->direction = glm::normalize(pSpotLight->direction);
	pSpotLight->diffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    pSpotLight->attenuationConstant = 0.1f;
	pSpotLight->attenuationLinear = 0.01f;
	pSpotLight->attenuationQuadratic = 0.001f;

    // Cone angles
    pSpotLight->spotLightInnerAngle = 15.0f;	// Degrees
    pSpotLight->spotLightOuterAngle = 45.0f;

	cLight* pFlashLight = &(::g_pLightManager->myLights[1]);
	pFlashLight->lightType = cLight::SPOT_LIGHT;
	pFlashLight->bIsOn = true;
    

    pFlashLight->attenuationConstant = 0.1f;
    pFlashLight->attenuationLinear = 0.01f;
    pFlashLight->attenuationQuadratic = 0.001f;


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while ( ! glfwWindowShouldClose(window) )
    {
        // Flashlight that follows the camera
        pFlashLight->position = g_pFlyCamera->getEyeLocation();

        glm::vec3 cameraLookAt = g_pFlyCamera->getTargetLocation();
        glm::vec3 cameraDirection = cameraLookAt - g_pFlyCamera->getEyeLocation();
        pFlashLight->direction = glm::normalize(cameraDirection);



        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);

        // Clears the screen
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Enable depth buffer (and culling)
        // - Add DEPTH_BUFFER_BIT to clear depth buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    

        glEnable(GL_DEPTH_TEST);    // Turn on depth checking
        glDepthFunc(GL_LESS);       // The depth compare function

        
        // Pass the eye (camera) location to the shader for specular
        // uniform vec3 eyeLocation;
        GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
        glm::vec3 eyeLocationXYZ = ::g_pFlyCamera->getEyeLocation();
        glUniform3f(eyeLocation_UL,
                    eyeLocationXYZ.x, eyeLocationXYZ.y, eyeLocationXYZ.z);


        // "p" for "projection"
        glm::mat4 matProj 
            = glm::perspective( glm::radians(60.0f),        // FOV 
                                (float)width / (float)height, // Aspect ratio
                                0.1f,            // Near plane
                                1000.0f);         // Far plane

        // the "camera"
        glm::mat4 matView 
            = glm::lookAt( ::g_pFlyCamera->getEyeLocation(),    //  ::g_eyePosition,
                           ::g_pFlyCamera->getTargetLocation(), // ::g_atPosition,
                           ::g_upAxis);

        // uniform mat4 mView;
        GLint mView_location = glGetUniformLocation(program, "mView");
        glUniformMatrix4fv(mView_location,
            1,
            GL_FALSE,
            (const GLfloat*)&matView);

        // uniform mat4 mProject;
        GLint mProject_location = glGetUniformLocation(program, "mProject");
        glUniformMatrix4fv(mProject_location,
            1,
            GL_FALSE,
            (const GLfloat*)&matProj);


        // Copy light info to shader for this frame
        ::g_pLightManager->CopyLightInfoToShader(program);

        std::vector< cMesh* > vec_transparentMeshes;
        std::vector< cMesh* > vec_SolidMeshes;


        // Sort meshes from transparent and solid into 2 new lists
        for (std::vector<cMesh*>::iterator it_pMesh = ::g_vec_pMeshes.begin();
            it_pMesh != ::g_vec_pMeshes.end();
            it_pMesh++)
        {

            cMesh* pTheMesh = *it_pMesh;

            if (pTheMesh->alphaTransparency < 1.0f) 
            {
                vec_transparentMeshes.push_back(pTheMesh);
            }
            else 
            {
                vec_SolidMeshes.push_back(pTheMesh);
            }

        }


        // Now draw all the solid objects first
        for (std::vector<cMesh*>::iterator it_pMesh = vec_SolidMeshes.begin();
             it_pMesh != vec_SolidMeshes.end();
             it_pMesh++)
        {

            cMesh* pTheMesh = *it_pMesh;            // Gives us pointer to mesh

            DrawObject(pTheMesh, program);

        }

        // Now, sort the transparent meshes by distance from camera
        // TODO: Sort transparent object from farthest to closest to camera


        // Draw all trasnparent meshes, they now should be sorted
        for (std::vector<cMesh*>::iterator it_pMesh = vec_transparentMeshes.begin();
            it_pMesh != vec_transparentMeshes.end();
            it_pMesh++)
        {
     
            cMesh* pTheMesh = *it_pMesh;           

            DrawObject(pTheMesh, program);
        }


        // *******************************************************
        // DRAW DEBUG SPHERES AROUND LIGHTS
        if (::g_pShowDebugLightSpheres)
        {

            cMesh* pDebugSphere = g_pFindMeshByFriendlyName("DebugSphere1");
            pDebugSphere->bIsVisible = true;
            pDebugSphere->bIsWireFrame = true;
            pDebugSphere->bDoNotLight = true;

            pDebugSphere->position = g_pLightManager->myLights[::g_SelectedLightIndex].position;
            pDebugSphere->diffuseRGB = glm::vec3(1.0f, 1.0f, 1.0f);
            pDebugSphere->scale = 0.2f;

            DrawObject(pDebugSphere, program);

            float debugSphereBrightness = 0.5f;

            cLightHelper theLightHelper;

            // Draw a sphere that shows where the light is at 75% brightness
            float distanceAt75PercentBrightness
                = theLightHelper.calcApproxDistFromAtten(
                    0.75f,   // 75% brightness
                    0.01f,   // To within 0.01f
                    100000.0f,  // Up to 100,000 units from the origin
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationConstant,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic);

            pDebugSphere->diffuseRGB = glm::vec3(debugSphereBrightness, debugSphereBrightness, 0.0f);
            pDebugSphere->scale = distanceAt75PercentBrightness;
            DrawObject(pDebugSphere, program);

            float distanceAt50PercentBrightness
                = theLightHelper.calcApproxDistFromAtten(
                    0.50f,   // 75% brightness
                    0.01f,   // To within 0.01f
                    100000.0f,  // Up to 100,000 units from the origin
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationConstant,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic);

            pDebugSphere->diffuseRGB = glm::vec3(debugSphereBrightness, 0.0f, 0.0f);
            pDebugSphere->scale = distanceAt50PercentBrightness;
            DrawObject(pDebugSphere, program);


            float distanceAt25PercentBrightness
                = theLightHelper.calcApproxDistFromAtten(
                    0.25f,   // 75% brightness
                    0.01f,   // To within 0.01f
                    100000.0f,  // Up to 100,000 units from the origin
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationConstant,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic);

            pDebugSphere->diffuseRGB = glm::vec3(0.0f, debugSphereBrightness, 0.0f);
            pDebugSphere->scale = distanceAt25PercentBrightness;
            DrawObject(pDebugSphere, program);


            float distanceAt5PercentBrightness
                = theLightHelper.calcApproxDistFromAtten(
                    0.05f,   // 75% brightness
                    0.01f,   // To within 0.01f
                    100000.0f,  // Up to 100,000 units from the origin
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationConstant,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear,
                    g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic);

            pDebugSphere->diffuseRGB = glm::vec3(0.0f, debugSphereBrightness, debugSphereBrightness);
            pDebugSphere->scale = distanceAt5PercentBrightness;
            DrawObject(pDebugSphere, program);


            pDebugSphere->bIsVisible = false;
        }//if (::g_pShowDebugLightSpheres)
        // *******************************************************


        // Present what we drew to the screen..
        glfwSwapBuffers(window);
        
        // GLFW checks the mouse and keyboard
        glfwPollEvents();
        //
        handleKeyboardAsync(window);
        handleMouseAsync(window);


        std::stringstream ssTitle;

        glm::vec3 eyePosition = ::g_pFlyCamera->getEyeLocation();

        ssTitle << "Camera (x,y,z): "
            << eyePosition.x << ", "
            << eyePosition.y << ", "
            << eyePosition.z;

        ssTitle << "  Light[" << ::g_SelectedLightIndex << "] @"
            << ::g_pLightManager->myLights[::g_SelectedLightIndex].position.x << ", "
            << ::g_pLightManager->myLights[::g_SelectedLightIndex].position.y << ", "
            << ::g_pLightManager->myLights[::g_SelectedLightIndex].position.z << " "
            << "Lin. Atten.: " << ::g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear << " "
            << "Quad. Atten.: " << ::g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic;


        // str() makes it a std::string
        // c_str() makes it a char* ("C style string")
        glfwSetWindowTitle(window, ssTitle.str().c_str() );


    }//  while(...)

    delete pTheShaderManager;
    delete ::g_pVAOManager;
    delete ::g_pFlyCamera;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}



float getRand(void)
{
    return ((float)rand()) / RAND_MAX;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    // Check if ONLY shift is down
    if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {

        // Use O & P to select the light
        if (key == GLFW_KEY_O && action == GLFW_PRESS)
        {
            // Select previous light
            ::g_SelectedLightIndex--;
            // 
            if (::g_SelectedLightIndex < 0)
            {
                ::g_SelectedLightIndex = cLightManager::NUMBEROF_LIGHTS - 1;
            }
        }
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            // Select next light
            ::g_SelectedLightIndex++;
            //
            if (::g_SelectedLightIndex >= cLightManager::NUMBEROF_LIGHTS)
            {
                ::g_SelectedLightIndex = 0;
            }
        }

        // Turn off and on the debug spheres
        if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        {
            // Turn on the debug spheres
            ::g_pShowDebugLightSpheres = true;
        }
        if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        {
            // Turn off the debug spheres
            ::g_pShowDebugLightSpheres = false;
        }


    }//if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)



    const float CAMERA_MOVE_SPEED = 0.1f;

 //   // WASD+QE
 //   // Left and Right 
 //   if (key == GLFW_KEY_A)    // Left (-ve on X)
 //   {
 //       ::g_eyePosition.x -= CAMERA_MOVE_SPEED;
 //   }
 //   if (key == GLFW_KEY_D)     // Right (+ve on X)
 //   {
 //       ::g_eyePosition.x += CAMERA_MOVE_SPEED;
 //   }
 //
 //   if (key == GLFW_KEY_W)      // Forward  (+ve on Z axis)
 //   {
 //       ::g_eyePosition.z += CAMERA_MOVE_SPEED;
 //   }
 //   if (key == GLFW_KEY_S)      // Backwards  (-ve on Z axis)
 //   {
 //       ::g_eyePosition.z -= CAMERA_MOVE_SPEED;
 //   }
 //
 //   if (key == GLFW_KEY_Q)      // Up   (+ve on Y axis)
 //   {
 //       ::g_eyePosition.y += CAMERA_MOVE_SPEED;
 //   }
 //   if (key == GLFW_KEY_E)      // Down (-ve on Y axis)
 //   {
 //       ::g_eyePosition.y -= CAMERA_MOVE_SPEED;
 //   }



    return;
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}




// Reutrn NULL if isn't found
cMesh* g_pFindMeshByFriendlyName(std::string nameToFind)
{
    // Up to about 100-200 object is faster using a linear search
    for (std::vector<cMesh*>::iterator it_pMesh = ::g_vec_pMeshes.begin();
        it_pMesh != ::g_vec_pMeshes.end();
        it_pMesh++)
    {
        cMesh* pTheMesh = *it_pMesh;

        // Is this it? 
        if (pTheMesh->friendlyName == nameToFind)
        {
            return pTheMesh;
        }
    }
    // Didn't find it 
    return NULL;
}