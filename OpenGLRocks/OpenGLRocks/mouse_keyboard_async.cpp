// for non keyboard callback handling

#include "../globalOpenGLStuff.h"       // Just the OpenGL stuff
#include "../globalStuff.h"             // Global items (like the camera)
 
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>

//extern glm::vec3 g_cameraEye;


int g_SelectedLightIndex = 0;

struct sMouseState
{
    glm::ivec2 currentPositionXY = glm::ivec2(0, 0);
    glm::ivec2 lastPositionXY = glm::ivec2(0, 0);
    bool bIsLeftMouseButtonDown = false;
};

sMouseState g_MouseState;

bool isShiftDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || 
        (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) )
    {
        return true;
    }
    return false;
}

bool isControlDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool isAltDown(GLFWwindow* window)
{
    if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
    {
        return true;
    }
    return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
    if ( isShiftDown(window) ) 
    {
        return false;
    }

    if ( isControlDown(window) ) 
    {
        return false;
    }

    if ( isAltDown(window) ) 
    {
        return false;
    }

    // Nothing is down
    return true;
}

void handleMouseAsync(GLFWwindow* window)
{

//    if (glfwRawMouseMotionSupported())
//        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


//    double xpos, ypos;
//    glfwGetCursorPos(window, &xpos, &ypos);
//
//    std::cout << "Async mouse location (x,y): " << xpos << ", " << ypos << std::endl;

    return;
}

void handleKeyboardAsync(GLFWwindow* window)
{
    const float CAMERA_MOVE_SPEED = 5.0f;
    const float CAMERA_TURN_SPEED = 1.0f;

    const float LIGHT_MOVE_SPEED = 0.1f;

    if ( isShiftDown(window) )
    {
        // Move the selected light
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].position.z += LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].position.x += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].position.y += LIGHT_MOVE_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
        }

        // Also change the attenuation
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            // Decrease the linear attenuation
            ::g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear *= 0.99f;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            // Decrease the linear attenuation
            ::g_pLightManager->myLights[::g_SelectedLightIndex].attenuationLinear *= 1.01f;
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            // Decrease the linear attenuation
            ::g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic *= 0.999f;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            // Decrease the linear attenuation
            ::g_pLightManager->myLights[::g_SelectedLightIndex].attenuationQuadratic *= 1.001f;
        }

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            ::g_pLightManager->myLights[::g_SelectedLightIndex].bIsOn = !::g_pLightManager->myLights[::g_SelectedLightIndex].bIsOn;
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            // Increase spotlight inner angle
            ::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightInnerAngle += 0.25f;

			if (::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightInnerAngle > 90.0f)
			{
				::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightInnerAngle = 90.0f;
			}
            

			if (::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightInnerAngle > ::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightOuterAngle)
			{
				::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightOuterAngle = ::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightInnerAngle;
			}
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            // Decrease spotlight inner angle
            ::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightInnerAngle -= 0.25f;
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            // Increase spotlight outer angle
            ::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightOuterAngle += 0.25f;

			if (::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightOuterAngle > 90.0f)
			{
				::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightOuterAngle = 90.0f;
			}
        }

        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            // Decrease spotlight outer angle
            ::g_pLightManager->myLights[::g_SelectedLightIndex].spotLightOuterAngle -= 0.25f;
        }


    }

    if (areAllModifiersUp(window))
    {

        // Move the camera 
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            // Go forward
//            ::g_cameraEye.z -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveForward( CAMERA_MOVE_SPEED );
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            // Go back
//            ::g_cameraEye.z += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveForward( -CAMERA_MOVE_SPEED );
        }

        if ( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS )
        {
            // Go left
//           ::g_cameraEye.x -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveLeftRight( CAMERA_MOVE_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            // Go right
//            ::g_cameraEye.x += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveLeftRight( -CAMERA_MOVE_SPEED );
        }



        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // Go down
//            ::g_cameraEye.y -= CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveUpDown( -CAMERA_MOVE_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            // Go up
//            ::g_cameraEye.y += CAMERA_MOVE_SPEED;
            ::g_pFlyCamera->moveUpDown( CAMERA_MOVE_SPEED );
        }

        // Use the arrow keys to turn and pitch the camera

        // Left pitches negative (rotate around Y a negative value)
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( CAMERA_TURN_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( -CAMERA_TURN_SPEED );
        }

         // Left pitches negative (rotate around Y a negative value)
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            ::g_pFlyCamera->pitchUpDown( -CAMERA_TURN_SPEED );
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            ::g_pFlyCamera->pitchUpDown( CAMERA_TURN_SPEED );
        }

    }//if (areAllModifiersUp(window)

    return;
}

// *********************************************************
// Mouse callbacks:

// Set with glfwSetCursorPosCallback(window, cursor_position_callback);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//    std::cout << "mouse x,y: " << xpos << ", " << ypos << std::endl;
    
    g_MouseState.currentPositionXY.x = (int)xpos;
    g_MouseState.currentPositionXY.y = (int)ypos;

    if ( g_MouseState.bIsLeftMouseButtonDown )
    {
        // If the Left mouse key is down, 
        //  figure out the differene between the current mouse postion and the last one

        glm::ivec2 deltaMousePosition = g_MouseState.lastPositionXY - g_MouseState.currentPositionXY;

        // Use those values to "turn" the camera
    
        // Move left or right? 
        if ( deltaMousePosition.x != 0 )
        {
            ::g_pFlyCamera->rotateLeftRight_Yaw( (float)deltaMousePosition.x );
        }
        // Mouse move Up or down? 
        if ( deltaMousePosition.y != 0 )
        {
            ::g_pFlyCamera->pitchUpDown((float)deltaMousePosition.y);
        }
    }//if ( g_MouseState.bIsLeftMouseButtonDown )


    // Update the last one
    g_MouseState.lastPositionXY = g_MouseState.currentPositionXY;

    //return;
}

// Set with glfwSetCursorEnterCallback(window, cursor_enter_callback);
void cursor_enter_callback(GLFWwindow* window, int entered)
{
    //if ( entered )
    //{
    //    std::cout << "Mouse is over the window" << std::endl;
    //}
    //else
    //{
    //    std::cout << "Mouse left the window" << std::endl;
    //}
    return;
}


// Set with glfwSetMouseButtonCallback(window, mouse_button_callback);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        switch (action)
        {
        case GLFW_PRESS:
//            std::cout << "GLFW_PRESS" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = true;
            break;
        case GLFW_RELEASE:
//            std::cout << "GLFW_RELEASE" << std::endl;
            g_MouseState.bIsLeftMouseButtonDown = false;
            break;
        default:
            std::cout << "Mouse button state unknown" << std::endl;
            break;
        }
    }

    return;
}

// Set with glfwSetScrollCallback(window, scroll_callback);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // For most mice, the value is the DIFFERENCE between the last time this was called!
    // (i.e. it's not an absolute value)
    // It also goes up and down along the Y axis and is an integer
 //   std::cout << "xoffset, yoffset: " << xoffset << ", " << yoffset << std::endl;

    const float MOUSE_WHEEL_RATIO = 0.01f;

    ::g_pFlyCamera->adjustMovementSpeed( float(yoffset) * MOUSE_WHEEL_RATIO );

    return;
}



