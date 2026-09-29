#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "settings.h"
#include "game.h"

//definitions of window variables
GLFWwindow* window = NULL;

int windowWidth;
int windowHeight;

//every time use change window size, window size is updated
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    processGameKeyboardInput(key, action);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    processGameMouseInput(xPos, yPos);
}

bool windowInit()
{
    windowWidth = startWindowWidth;
    windowHeight = startWindowHeight;

    //everything here is from settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    free((void*)windowTitle);
    if(window == NULL)
    {
        printf("Failed to create window object\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    return true;
}
