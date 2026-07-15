#include "globalOpenGLStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cMesh.h"
#include "cVAOManager/cVAOManager.h"

// This is in theMain.cpp
extern cVAOManager* g_pVAOManager;

// "program" is the OpenGL shader progrma
void DrawObject(cMesh* pTheMesh, GLuint program)
{

    // is it visible
    if ( ! pTheMesh->bIsVisible )
    {
        return;
    }

    // Set to the "identity matrix"
    glm::mat4 matModel = glm::mat4(1.0f);         // "m"    //mat4x4 m, p, mvp;


    // Transformations 
    glm::mat4 matTranslate
        = glm::translate(glm::mat4(1.0f),
            glm::vec3(pTheMesh->position.x,
                pTheMesh->position.y,
                pTheMesh->position.z));

    glm::mat4 matRotateX
        = glm::rotate(glm::mat4(1.0f),
            glm::radians<float>(pTheMesh->rotation.x),
            glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 matRotateY
        = glm::rotate(glm::mat4(1.0f),
            glm::radians<float>(pTheMesh->rotation.y),
            glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 matRotateZ
        = glm::rotate(glm::mat4(1.0f),
            glm::radians<float>(pTheMesh->rotation.z),
            glm::vec3(0.0f, 0.0f, 1.0f));


    glm::mat4 matScale
        = glm::scale(glm::mat4(1.0f),
            glm::vec3(pTheMesh->scale,
                pTheMesh->scale,
                pTheMesh->scale));


    // combine the rotation
    matModel = matScale * matModel;         // last to be applied

    matModel = matRotateX * matModel;
    matModel = matRotateY * matModel;
    matModel = matRotateZ * matModel;

    matModel = matTranslate * matModel;     // 1st to be applied

    // uniform mat4 mModel;
    GLint mModel_location = glGetUniformLocation(program, "mModel");
    glUniformMatrix4fv(mModel_location,
        1,
        GL_FALSE,
        (const GLfloat*)&matModel);
    //mvp  -- pvm
    //            glm::mat4x4 mvp = matProj * matView * matModel;
    //
    //            const GLint mvp_location = glGetUniformLocation(program, "MVP");
    //            glUniformMatrix4fv( mvp_location,
    //                                1, 
    //                                GL_FALSE, 
    //                                (const GLfloat*)&mvp);


                //        glPointSize(10.0f);
    if (pTheMesh->bIsWireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

	GLint alpha_UL = glGetUniformLocation(program, "alphaTransparency");

    glUniform1f(alpha_UL, pTheMesh->alphaTransparency);


    // Get the uniform for the colour
    // uniform vec3 theColour;
    GLint theColour_UL = glGetUniformLocation(program, "theColour");

    glUniform3f(theColour_UL,
        pTheMesh->diffuseRGB.r,
        pTheMesh->diffuseRGB.g,
        pTheMesh->diffuseRGB.b);


    // uniform bool bDoNotLight;
    GLint bDoNotLight_UL = glGetUniformLocation(program, "bDoNotLight");
    if (pTheMesh->bDoNotLight)
    {
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_TRUE);  // 1.0f
    }
    else
    {   // Normal lighting
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_FALSE);  // 0.0f
    }





    std::string meshNameToDraw = pTheMesh->meshName;

    sModelDrawInfo modelToDraw;
    if (g_pVAOManager->FindDrawInfoByModelName(meshNameToDraw, modelToDraw))
    {
        glBindVertexArray(modelToDraw.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelToDraw.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);              // Starting index

        glBindVertexArray(0);
    }//if ( g_pVAOManager

    return;
}
