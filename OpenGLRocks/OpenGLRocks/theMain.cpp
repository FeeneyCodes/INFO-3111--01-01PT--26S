//#define GLAD_GL_IMPLEMENTATION
//#include <glad/glad.h>
////#include "include/glad/glad.h"
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalStuff.h"

#include <iostream>

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
// String stream
#include <sstream> 
#include <iostream>     // cout
#include <fstream>      // File IO

#include "cShaderManager/cShaderManager.h"

struct Vertex
{
    glm::vec3 position;      // vec2 pos;  position
    glm::vec3 colour;        // vec3 col;    colour
};

static const Vertex vertices[6] =
{   //     X     Y       Z    R      G     B  
    //   float   float         float 
    { { -0.6f, -0.4f, 0.0f }, { 1.0f, 0.0f, 1.0f } },
    { {  0.6f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    { {  0.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
    { { -0.6f + 2.0f, -0.4f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { {  0.6f + 2.0f, -0.4f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    { {  0.0f + 2.0f,  0.6f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
};

//static const char* vertex_shader_text = 
//"#version 330\n"
//"uniform mat4 MVP;\n"
//"in vec3 vCol;\n"
//"in vec3 vPos;\n"
//"out vec3 color;\n"
//"void main()\n"
//"{\n"
//"    gl_Position = MVP * vec4(vPos, 1.0);\n"
//"    color = vCol;\n"
//"}\n";
//
//static const char* fragment_shader_text =
//"#version 330\n"
//"in vec3 color;\n"
//"out vec4 fragment;\n"
//"void main()\n"
//"{\n"
//"    fragment = vec4(color, 1.0);\n"
//"}\n";

glm::vec3 g_eyePosition = glm::vec3(0.0f, 0.0f, -1.0f);   // Camera location or position
glm::vec3 g_atPosition = glm::vec3(0.0f, 0.0f, 0.0f);// Looking "at" 
glm::vec3 g_upAxis = glm::vec3(0.0f, +1.0f, 0.0f);// What's up




static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    const float CAMERA_MOVE_SPEED = 0.01f;

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

int main(void)
{
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

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData( GL_ARRAY_BUFFER, 
                  sizeof(vertices), 
                  vertices, 
                  GL_STATIC_DRAW);


    cShaderManager* pTheShaderManager = new cShaderManager();

    cShaderManager::cShader vertexShader;
    vertexShader.fileName = "simpleVertex.glsl";

    cShaderManager::cShader fragmentShader;
    fragmentShader.fileName = "simpleFragment.glsl";

    pTheShaderManager->setBasePath("assets/shaders/");

    if ( ! pTheShaderManager->createProgramFromFile("SimpleShader",
                                                    vertexShader,
                                                    fragmentShader))
    {
        std::cout << "Shader error:" << pTheShaderManager->getLastError() << std::endl;
        return -1;
    }
    
    // Get the shader program ID (for later)
    GLuint program = pTheShaderManager->getIDFromFriendlyName("SimpleShader");

    //const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    //glCompileShader(vertex_shader);

    //const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    //glCompileShader(fragment_shader);

    //const GLuint program = glCreateProgram();
    //glAttachShader(program, vertex_shader);
    //glAttachShader(program, fragment_shader);
    //glLinkProgram(program);





    const GLint mvp_location = glGetUniformLocation(program, "MVP");


    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    //struct Vertex
    //{
    //    glm::vec3 position;      // vec2 pos;  position
    //    glm::vec3 colour;      // vec3 col;    colour
    //};
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer( vpos_location, 
                           3, 
                           GL_FLOAT, 
                           GL_FALSE,
                           sizeof(Vertex),      // 20
                           (void*)offsetof(Vertex, position));

    const GLint vcol_location = glGetAttribLocation(program, "vCol");
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer( vcol_location, 
                           3, 
                           GL_FLOAT, 
                           GL_FALSE,
                           sizeof(Vertex), 
                           (void*)offsetof(Vertex, colour));

    while ( ! glfwWindowShouldClose(window) )
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 m;             //mat4x4 m, p, mvp;
        glm::mat4 p;
        glm::mat4 mvp;
        //mat4x4_identity(m);
        
 //       mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f), 
                                         0.0f, //(float)glfwGetTime(),         // Angle
                                         glm::vec3(0.0f, 0.0f, 1.0f));
            
 //       mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
 
        //// Camera location or position
        //glm::vec3 eyePosition = glm::vec3( 0.0f, 0.0f, -1.0f);
        //// Looking "at" 
        //glm::vec3 atPosition = glm::vec3( 0.0f, 0.0f, 0.0f );
        //// What's up
        //glm::vec3 upAxis = glm::vec3( 0.0f, +1.0f, 0.0f );

 //       eyeZValue += 0.005f;

        // the "camera"
        glm::mat4 matView = glm::lookAt(::g_eyePosition, 
                                        ::g_atPosition, 
                                        ::g_upAxis);

        // projection matrix
        p = glm::perspective( 60.0f,        // FOV
                              (float)width / (float)height, // Aspect ratio
                              0.1f,            // Near plane
                              1000.0f);         // Far plane
                              

 //       mat4x4_mul(mvp, p, m);
        m = glm::mat4(1.0f);        // Identity matrix
        // combine the rotation
        m = rotateZ * m;
        
        //mvp  -- pvm
        mvp = p * matView * m;

        glUseProgram(program);

        glUniformMatrix4fv( mvp_location, 
                            1, 
                            GL_FALSE, 
                            (const GLfloat*)&mvp);

        glBindVertexArray(vertex_array);
//        glPointSize(10.0f);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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