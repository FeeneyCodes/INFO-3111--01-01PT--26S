//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
////#include "include/glad/glad.h"
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
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


glm::vec3 g_eyePosition = glm::vec3(0.0f, 0.0f, -5.0f);   // Camera location or position
glm::vec3 g_atPosition = glm::vec3(0.0f, 0.0f, 0.0f);// Looking "at" 
glm::vec3 g_upAxis = glm::vec3(0.0f, +1.0f, 0.0f);// What's up

//
cVAOManager* g_pVAOManager = NULL;

// note these are pointers
std::vector< cMesh* > g_vec_pMeshes;

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "I love OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

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
    if (::g_pVAOManager->LoadModelIntoVAO("de--lorean.ply", carModel, program))
    {
        std::cout << "Loaded " << carModel.meshName << std::endl;
        std::cout << "\t" << carModel.numberOfVertices << " vertices" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Didn't load " << carModel.meshName << std::endl;
    }

    sModelDrawInfo cowModel;
    ::g_pVAOManager->LoadModelIntoVAO("cow.ply", cowModel, program);

    sModelDrawInfo mig29Model;
    ::g_pVAOManager->LoadModelIntoVAO("mig29_xyz_only.ply", mig29Model, program);

    sModelDrawInfo bunnyModel;
    ::g_pVAOManager->LoadModelIntoVAO("bun_zipper_xyz.ply", bunnyModel, program);

    // Add the models we want to draw
    cMesh* pCow = new cMesh();
    pCow->meshName = "cow.ply";
    pCow->position = glm::vec3(0.0f, 0.0f, -2.0f);
    pCow->scale = 1.0f / 10.0f;
    pCow->bIsWireFrame = true;
    pCow->diffuseRGB = glm::vec3(0.0f, 0.0f, 1.0f);

    cMesh* pPlane = new cMesh();
    pPlane->meshName = "mig29_xyz_only.ply";
    pPlane->position = glm::vec3(0.0f, 2.0f, 0.0f);
    pPlane->rotation.x = 45.0f;
    pPlane->rotation.y = 15.0f;
    pPlane->scale = 2.0f;
    pPlane->diffuseRGB = glm::vec3(1.0f, 0.0f, 0.0f);

    cMesh* pBunny1 = new cMesh();
    pBunny1->meshName = "bun_zipper_xyz.ply";
    pBunny1->position.x = 2.0f;
    pBunny1->diffuseRGB = glm::vec3(0.6f, 0.3f, 0.7f);

    cMesh* pBunny2 = new cMesh();
    pBunny2->meshName = "bun_zipper_xyz.ply";
    pBunny2->position.x = 3.0f;
    pBunny2->scale = 1.5f;
    pBunny1->diffuseRGB = glm::vec3(0.3f, 0.7f, 0.6f);

    //cMesh* pCar = new cMesh();
    //pCar->meshName = "de--lorean.ply";
    //pCar->position = glm::vec3(0.0f, 0.0f, 0.0f);

    ::g_vec_pMeshes.push_back(pCow);
    ::g_vec_pMeshes.push_back(pPlane);
    ::g_vec_pMeshes.push_back(pBunny1);
    ::g_vec_pMeshes.push_back(pBunny2);
 //   ::g_vec_pMeshes.push_back(pCar);



    while ( ! glfwWindowShouldClose(window) )
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);

        // Clears the screen
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // "p" for "projection"
        glm::mat4 matProj 
            = glm::perspective( glm::radians(60.0f),        // FOV 
                                (float)width / (float)height, // Aspect ratio
                                0.1f,            // Near plane
                                1000.0f);         // Far plane

        // the "camera"
        glm::mat4 matView 
            = glm::lookAt( ::g_eyePosition,
                           ::g_atPosition,
                           ::g_upAxis);



        for (std::vector<cMesh*>::iterator it_pMesh = ::g_vec_pMeshes.begin();
             it_pMesh != ::g_vec_pMeshes.end(); 
             it_pMesh++)
        {

            cMesh* pTheMesh = *it_pMesh;            // Gives us pointer to mesh


            // Set to the "identity matrix"
            glm::mat4 matModel = glm::mat4(1.0f);         // "m"    //mat4x4 m, p, mvp;
       

            // Transformations 
            glm::mat4 matTranslate
                = glm::translate(glm::mat4(1.0f),
                    glm::vec3( pTheMesh->position.x,
                               pTheMesh->position.y,
                               pTheMesh->position.z) );

            glm::mat4 matRotateX
                = glm::rotate( glm::mat4(1.0f),
                               glm::radians<float>(pTheMesh->rotation.x),
                               glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 matRotateY  
                = glm::rotate( glm::mat4(1.0f),
                               glm::radians<float>(pTheMesh->rotation.y),
                               glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 matRotateZ 
                = glm::rotate( glm::mat4(1.0f),
                               glm::radians<float>(pTheMesh->rotation.z),
                               glm::vec3(0.0f, 0.0f, 1.0f));


            glm::mat4 matScale
                = glm::scale(glm::mat4(1.0f),
                    glm::vec3( pTheMesh->scale,
                               pTheMesh->scale,
                               pTheMesh->scale) );

      
            // combine the rotation
            matModel = matScale * matModel;         // last to be applied
        
            matModel = matRotateX * matModel;
            matModel = matRotateY * matModel;
            matModel = matRotateZ * matModel;

            matModel = matTranslate * matModel;     // 1st to be applied


            //mvp  -- pvm
            glm::mat4x4 mvp = matProj * matView * matModel;


            const GLint mvp_location = glGetUniformLocation(program, "MVP");
            glUniformMatrix4fv( mvp_location,
                                1, 
                                GL_FALSE, 
                                (const GLfloat*)&mvp);


            //        glPointSize(10.0f);
            if (pTheMesh->bIsWireFrame)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }


            // Get the uniform for the colour
            // uniform vec3 theColour;
            GLint theColour_UL = glGetUniformLocation(program, "theColour");

            glUniform3f( theColour_UL,
                         pTheMesh->diffuseRGB.r,
                         pTheMesh->diffuseRGB.g,
                         pTheMesh->diffuseRGB.b);
 

            std::string meshNameToDraw = pTheMesh->meshName;

            sModelDrawInfo modelToDraw;
            if ( g_pVAOManager->FindDrawInfoByModelName(meshNameToDraw, modelToDraw) )
            {
                glBindVertexArray(modelToDraw.VAO_ID);

                glDrawElements(GL_TRIANGLES, 
                               modelToDraw.numberOfIndices, 
                               GL_UNSIGNED_INT, 
                               (void*)0);              // Starting index

                glBindVertexArray(0);
            }//if ( g_pVAOManager



        }//for (std::vector<cMesh*>




        glfwSwapBuffers(window);
        glfwPollEvents();


        std::stringstream ssTitle;

        ssTitle << "Camera (x,y,z): "
            << ::g_eyePosition.x << ", "
            << ::g_eyePosition.y << ", "
            << ::g_eyePosition.z;

        // str() makes it a std::string
        // c_str() makes it a char* ("C style string")
        glfwSetWindowTitle(window, ssTitle.str().c_str() );


    }//  while(...)

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

    const float CAMERA_MOVE_SPEED = 0.1f;

    // WASD+QE
    // Left and Right 
    if (key == GLFW_KEY_A)    // Left (-ve on X)
    {
        ::g_eyePosition.x -= CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_D)     // Right (+ve on X)
    {
        ::g_eyePosition.x += CAMERA_MOVE_SPEED;
    }

    if (key == GLFW_KEY_W)      // Forward  (+ve on Z axis)
    {
        ::g_eyePosition.z += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_S)      // Backwards  (-ve on Z axis)
    {
        ::g_eyePosition.z -= CAMERA_MOVE_SPEED;
    }

    if (key == GLFW_KEY_Q)      // Up   (+ve on Y axis)
    {
        ::g_eyePosition.y += CAMERA_MOVE_SPEED;
    }
    if (key == GLFW_KEY_E)      // Down (-ve on Y axis)
    {
        ::g_eyePosition.y -= CAMERA_MOVE_SPEED;
    }



    return;
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

