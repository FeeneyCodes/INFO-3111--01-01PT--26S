#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
//#include "include/glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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

typedef struct Vertex
{
    glm::vec2 pos;      // vec2 pos;
    glm::vec3 col;      // vec3 col;
} Vertex;

static const Vertex vertices[24] =
{
    // Triangle 1 - right lower
    { {  3.0f,  0.0f }, { 1.0f, 1.0f, 1.0f } },
    { {  1.2f, -1.2f }, { 0.0f, 1.0f, 0.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 2 - bottom right
    { {  1.2f, -1.2f }, { 1.0f, 0.5f, 0.0f } },
    { {  0.0f, -3.0f }, { 0.0f, 1.0f, 1.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 3 - bottom left
    { {  0.0f, -3.0f }, { 0.0f, 0.0f, 1.0f } },
    { { -1.2f, -1.2f }, { 1.0f, 1.0f, 0.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 4 - left lower
    { { -1.2f, -1.2f }, { 1.0f, 1.0f, 1.0f } },
    { { -3.0f,  0.0f }, { 1.0f, 0.0f, 0.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 5 - left upper
    { { -3.0f,  0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { -1.2f,  1.2f }, { 0.0f, 1.0f, 0.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 6 - top left
    { { -1.2f,  1.2f }, { 1.0f, 0.5f, 0.0f } },
    { {  0.0f,  3.0f }, { 0.0f, 1.0f, 1.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 7 - top right
    { {  0.0f,  3.0f }, { 0.0f, 0.0f, 1.0f } },
    { {  1.2f,  1.2f }, { 1.0f, 1.0f, 0.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } },

    // Triangle 8 - right upper
    { {  1.2f,  1.2f }, { 0.0f, 1.0f, 0.0f } },
    { {  3.0f,  0.0f }, { 1.0f, 0.0f, 0.0f } },
    { {  0.0f,  0.0f }, { 1.0f, 0.0f, 1.0f } }
};

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

float eyeZValue = -1.0f;

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, col));

    while (!glfwWindowShouldClose(window))
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
                                         (float)glfwGetTime(),         // Angle
                                         glm::vec3(0.0f, 0.0f, 1.0f));
            

 //       mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
 
        glm::vec3 eyePosition = glm::vec3( 0.0f, 0.0f, eyeZValue);
        glm::vec3 atPosition = glm::vec3( 0.0f, 0.0f, 0.0f );
        glm::vec3 upAxis = glm::vec3( 0.0f, +1.0f, 0.0f );

 //       eyeZValue += 0.005f;

        // the "camera"
        glm::mat4 matView = glm::lookAt(eyePosition, atPosition, upAxis);

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
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}