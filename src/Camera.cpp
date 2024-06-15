#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void Camera::Move(double dx, double dy){
    view = glm::translate(view, glm::vec3(dx,dy, 0.f));
}

void Camera::Rotate(double dx, double dy){
    //É necessário determinar a direção para a qual o mouse se moveu para encontrar
    //o eixo sobre o qual rotacionar utilizando produto vetorial
    glm::vec3 direction = glm::normalize(point - position);
    glm::vec3 directionDx = direction;
    //Invertendo a direção de deslocamente, uma vez que deseja-se rotacionar na direção
    //oposta à qual o mouse se moveu
    directionDx.x -= dx;
    directionDx.y -= dy;
    directionDx = glm::normalize(directionDx);
    glm::vec3 axis = glm::cross(direction,directionDx);
    double dotproduct = glm::dot(direction,directionDx);
    //Encontrando o ângulo de rotação
    double angle = glm::acos(dotproduct);
    cout << "Dot product: " << dotproduct << "Angle: " << (float)angle << endl;
    cout << "Axis: " << "X: " << axis.x << " Y: " << axis.y << " Z: " << axis.z << endl;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, (float)angle/10, axis);
    position = glm::vec4(position.x,position.y,position.z, 0.0f)*model;
    up = glm::vec4(up.x,up.y,up.z, 0.0f)*model;
    // position.x = result.x;
    // position.y = result.y;
    // position.z = result.z;
    cout << "Position: " <<"X: " <<position.x << " Y: " << position.y << " Z: " << position.z << endl;
    cout << "Up: " <<"X: " << up.x << " Y: " << up.y << " Z: " << up.z << endl;
    cout << "Is it still up?!: " << glm::dot(position,up) << endl;
}

void Camera::processMouseInput(){
    mouse.Update();
    bool left;
    bool right;
    double dx;
    double dy;
    mouse.getButtonsPressed(left, right);
    mouse.getDistance(dx,dy);
    if(sqrt((dx*dx) + (dy*dy)) >= 0.01){
        if(left)
            Rotate(dx, dy);
        if(right)
            Move(dx,dy);
    }
}

void Camera::Update(){
    processMouseInput();
}