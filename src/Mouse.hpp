#pragma once
#include <glfw/include/GLFW/glfw3.h>

//TODO: TRANSFORMAR MOUSE EM SINGLETON
//TODO: IMPLEMENTAR SENSIBILIDADE DO MOUSE
class Mouse{
    private:

    double PositionX, PositionY;
    double distanceX, distanceY;
    bool buttons[2];

    GLFWwindow * window;

    void getMouseButtonInput();

    public:

    //Funções de callback não funcionam muito bem com POO
    static double scrollOffset;

    Mouse(GLFWwindow * window);

    void Update();

    void getButtonsPressed(bool &left, bool &right){
        left = buttons[0];
        right = buttons[1];
    }

    void getDistance(double & x, double & y){
        x = distanceX;
        y = distanceY;
    }

};