#include "Mouse.hpp"
#include <iostream>

using namespace std;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

double Mouse::scrollOffset = 0;

void Mouse::getMouseButtonInput(){
    int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int rightState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    buttons[0] = leftState == GLFW_PRESS;
    buttons[1] = rightState == GLFW_PRESS;
}

Mouse::Mouse(GLFWwindow* window){
    this->window = window;
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    getMouseButtonInput();
    glfwGetCursorPos(window, &PositionX, &PositionY);
    glfwSetScrollCallback(window, scroll_callback);
}

void Mouse::Update(){
    double lastPositionX = PositionX;
    double lastPositionY = PositionY;
    glfwGetCursorPos(window, &PositionX, &PositionY);
    distanceX = (PositionX - lastPositionX)/800.;
    distanceY = (lastPositionY - PositionY)/600.;
    getMouseButtonInput();
    scrollOffset = 0.f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Mouse::scrollOffset = -yoffset;
}
